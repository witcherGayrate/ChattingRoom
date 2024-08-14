#include "ChatGrpcClient.h"

ChatConPool::ChatConPool(int pool_size, std::string host, std::string port):
	_pool_size(pool_size),_host(host),_port(port),_b_stop(false)
{
	//初始化rpc连接池
	for (int i = 0; i < _pool_size; i++)
	{
		//创建channel
		std::shared_ptr<Channel> channel = grpc::CreateChannel(_host+":"+_port,grpc::InsecureChannelCredentials());//创建一个不用安全验证的channel
		_connections.push(ChatService::NewStub(channel));//创建一个临时变量（右值）放入队列
	}
}

ChatConPool::~ChatConPool()
{ 
	//上锁
	std::lock_guard<std::mutex> lock(_mutex);
	Close();//设为关闭
	while (!_connections.empty())
	{
		_connections.pop();
	}
}

std::unique_ptr<ChatService::Stub> ChatConPool::getConnection()
{
	//上独占锁
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock,[this]()
		{
			if (_b_stop)
			{
				return true;//池子已关闭，则持有锁，向下执行
			}
			return !_connections.empty(); //池子为非空，持锁向下执行，否则解锁，挂起等待
		});
	//池子若为空
	if (_b_stop)
	{
		return nullptr;
	}
	auto context = std::move(_connections.front());//uniuqe只能移动
	_connections.pop();//空的con扔出去
	return context;
}

void ChatConPool::returnConnection(std::unique_ptr<ChatService::Stub> context)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_b_stop)
	{
		return;
	}
	_connections.push(std::move(context));//移动资源
	_cond.notify_one();
}

void ChatConPool::Close()
{
	_b_stop = true;
	_cond.notify_all();//唤醒所有等待的线程执行
}

AddFriendRsp ChatGrpcClient::NotifyAddFriend(std::string server_ip, const AddFriendReq& req)
{
	AddFriendRsp rsp;
	Defer defer([&rsp, &req]()
		{
			rsp.set_error(ErrorCodes::Success);
			rsp.set_applyuid(req.applyuid());
			rsp.set_touid(req.touid());

		});
	auto find_iter = _pools.find(server_ip);
	if (find_iter == _pools.end())
	{
		return rsp;
	}
	auto& pool = find_iter->second;
	ClientContext context;
	auto stub = pool->getConnection();
	Status status = stub->NotifyAddFriend(&context, req, &rsp);//通过stub远程调用rpc服务

	Defer condefer([&stub, this, &pool]() {
		pool->returnConnection(std::move(stub));
		});

	if (!status.ok())
	{
		rsp.set_error(ErrorCodes::RPCFailed);
		return rsp;
	}
	return rsp;
}

AuthFriendRsp ChatGrpcClient::NotifyAuthFriend(std::string server_ip, const AuthFriendReq& req)
{
	AuthFriendRsp rsp;
	rsp.set_error(ErrorCodes::Success);

	Defer defer([&rsp, &req]() {
		rsp.set_fromuid(req.fromuid());
		rsp.set_touid(req.touid());
		});

	auto find_iter = _pools.find(server_ip);
	if (find_iter == _pools.end()) {
		return rsp;
	}

	auto& pool = find_iter->second;
	ClientContext context;
	auto stub = pool->getConnection();
	Status status = stub->NotifyAuthFriend(&context, req, &rsp);
	Defer defercon([&stub, this, &pool]() {
		pool->returnConnection(std::move(stub));
		});

	if (!status.ok()) {
		rsp.set_error(ErrorCodes::RPCFailed);
		return rsp;
	}

	return rsp;

}

TextChatMsgRsp ChatGrpcClient::NotifyTextChatMsg(std::string server_ip, const TextChatMsgReq& req, const Json::Value& rtValue)
{
	TextChatMsgRsp rsp;//给消息发送者的回包
	rsp.set_error(ErrorCodes::Success);

	Defer defer([&rsp, &req]() {
		rsp.set_fromuid(req.fromuid());
		rsp.set_touid(req.touid());
		//拷贝json arry中的msg_data
		for (const auto& text_data : req.textmsgs())
		{
			TextChatData* new_msg = rsp.add_textmsgs();
			new_msg->set_msgid(text_data.msgid());
			new_msg->set_msgcontent(text_data.msgcontent());
		}
		});

	auto find_iter = _pools.find(server_ip);
	if (find_iter == _pools.end())
	{
		return rsp;
	}
	auto& pool = find_iter->second;
	ClientContext context;
	auto stub = pool->getConnection();
	Status status = stub->NotifyTextChatMsg(&context, req, &rsp);
	Defer defercon([&stub, this, &pool]() {
		pool->returnConnection(std::move(stub));
		});

	if (!status.ok())
	{
		rsp.set_error(ErrorCodes::RPCFailed);
		return rsp;
	}
	return rsp;
}

bool ChatGrpcClient::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo)
{
	////优先查redis中查询用户信息
	//std::string info_str = "";
	//bool b_base = RedisMgr::GetInstance()->Get(base_key, info_str);
	//if (b_base) {
	//	Json::Reader reader;
	//	Json::Value root;
	//	reader.parse(info_str, root);
	//	userinfo->uid = root["uid"].asInt();
	//	userinfo->name = root["name"].asString();
	//	userinfo->pwd = root["pwd"].asString();
	//	userinfo->email = root["email"].asString();
	//	userinfo->nick = root["nick"].asString();
	//	userinfo->desc = root["desc"].asString();
	//	userinfo->sex = root["sex"].asInt();
	//	userinfo->icon = root["icon"].asString();
	//	std::cout << "user login uid is  " << userinfo->uid << " name  is "
	//		<< userinfo->name << " pwd is " << userinfo->pwd << " email is " << userinfo->email << endl;
	//}
	//else {
	//	//redis中没有则查询mysql
	//	//查询数据库
	//	std::shared_ptr<UserInfo> user_info = nullptr;
	//	user_info = MysqlMgr::GetInstance()->GetUser(uid);
	//	if (user_info == nullptr) {
	//		return false;
	//	}

	//	userinfo = user_info;

	//	//将数据库内容写入redis缓存
	//	Json::Value redis_root;
	//	redis_root["uid"] = uid;
	//	redis_root["pwd"] = userinfo->pwd;
	//	redis_root["name"] = userinfo->name;
	//	redis_root["email"] = userinfo->email;
	//	redis_root["nick"] = userinfo->nick;
	//	redis_root["desc"] = userinfo->desc;
	//	redis_root["sex"] = userinfo->sex;
	//	redis_root["icon"] = userinfo->icon;
	//	RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());
	//}
	std::cout << "ChatGrpcClient::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo) :to do";
	return true;

}

ChatGrpcClient::ChatGrpcClient()
{
	//读取对端配置
	auto& cfg = ConfigMgr::Inst();
	auto server_list = cfg["PeerServer"]["Servers"];
	
	std::vector<std::string> words;

	//字符串处理流
	std::stringstream ss(server_list);
	std::string word;
	//行读取并，分割
	while (std::getline(ss, word, ','))
	{
		words.push_back(word);
	}

	//拿着server name找到对应的host和port
	for (auto& word : words)
	{
		if (cfg[word]["Name"].empty())
		{
			continue;
		}
		_pools[cfg[word]["Name"]] = std::make_unique<ChatConPool>(5, cfg[word]["Host"], cfg[word]["Port"]);
	}
}
