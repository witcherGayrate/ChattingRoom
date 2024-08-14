#include "ChatGrpcClient.h"

ChatConPool::ChatConPool(int pool_size, std::string host, std::string port):
	_pool_size(pool_size),_host(host),_port(port),_b_stop(false)
{
	//��ʼ��rpc���ӳ�
	for (int i = 0; i < _pool_size; i++)
	{
		//����channel
		std::shared_ptr<Channel> channel = grpc::CreateChannel(_host+":"+_port,grpc::InsecureChannelCredentials());//����һ�����ð�ȫ��֤��channel
		_connections.push(ChatService::NewStub(channel));//����һ����ʱ��������ֵ���������
	}
}

ChatConPool::~ChatConPool()
{ 
	//����
	std::lock_guard<std::mutex> lock(_mutex);
	Close();//��Ϊ�ر�
	while (!_connections.empty())
	{
		_connections.pop();
	}
}

std::unique_ptr<ChatService::Stub> ChatConPool::getConnection()
{
	//�϶�ռ��
	std::unique_lock<std::mutex> lock(_mutex);
	_cond.wait(lock,[this]()
		{
			if (_b_stop)
			{
				return true;//�����ѹرգ��������������ִ��
			}
			return !_connections.empty(); //����Ϊ�ǿգ���������ִ�У��������������ȴ�
		});
	//������Ϊ��
	if (_b_stop)
	{
		return nullptr;
	}
	auto context = std::move(_connections.front());//uniuqeֻ���ƶ�
	_connections.pop();//�յ�con�ӳ�ȥ
	return context;
}

void ChatConPool::returnConnection(std::unique_ptr<ChatService::Stub> context)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_b_stop)
	{
		return;
	}
	_connections.push(std::move(context));//�ƶ���Դ
	_cond.notify_one();
}

void ChatConPool::Close()
{
	_b_stop = true;
	_cond.notify_all();//�������еȴ����߳�ִ��
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
	Status status = stub->NotifyAddFriend(&context, req, &rsp);//ͨ��stubԶ�̵���rpc����

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
	TextChatMsgRsp rsp;//����Ϣ�����ߵĻذ�
	rsp.set_error(ErrorCodes::Success);

	Defer defer([&rsp, &req]() {
		rsp.set_fromuid(req.fromuid());
		rsp.set_touid(req.touid());
		//����json arry�е�msg_data
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
	////���Ȳ�redis�в�ѯ�û���Ϣ
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
	//	//redis��û�����ѯmysql
	//	//��ѯ���ݿ�
	//	std::shared_ptr<UserInfo> user_info = nullptr;
	//	user_info = MysqlMgr::GetInstance()->GetUser(uid);
	//	if (user_info == nullptr) {
	//		return false;
	//	}

	//	userinfo = user_info;

	//	//�����ݿ�����д��redis����
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
	//��ȡ�Զ�����
	auto& cfg = ConfigMgr::Inst();
	auto server_list = cfg["PeerServer"]["Servers"];
	
	std::vector<std::string> words;

	//�ַ���������
	std::stringstream ss(server_list);
	std::string word;
	//�ж�ȡ�����ָ�
	while (std::getline(ss, word, ','))
	{
		words.push_back(word);
	}

	//����server name�ҵ���Ӧ��host��port
	for (auto& word : words)
	{
		if (cfg[word]["Name"].empty())
		{
			continue;
		}
		_pools[cfg[word]["Name"]] = std::make_unique<ChatConPool>(5, cfg[word]["Host"], cfg[word]["Port"]);
	}
}
