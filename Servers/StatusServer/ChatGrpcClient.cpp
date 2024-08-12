#include "ChatGrpcClient.h"

ChatConPool::ChatConPool(int pool_size, std::string host, std::string port) :
	_pool_size(pool_size), _host(host), _port(port), _b_stop(false)
{
	//初始化rpc连接池
	for (int i = 0; i < _pool_size; i++)
	{
		//创建channel
		std::shared_ptr<Channel> channel = grpc::CreateChannel(_host + ":" + _port, grpc::InsecureChannelCredentials());//创建一个不用安全验证的channel
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
	_cond.wait(lock, [this]()
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
	return rsp;
}

//AuthFriendRsp ChatGrpcClient::NotifyAuthFriend(std::string server_ip, const AuthFriendReq& req)
//{
//	AuthFriendRsp rsp;
//	return rsp;
//}

//TextChatMsgRsp ChatGrpcClient::NotifyTextChatMsg(std::string server_ip, const TextChatMsgReq& req, const Json::Value& rtValue)
//{
//	TextChatMsgRsp rsp;
//	return rsp;
//}
//
//bool ChatGrpcClient::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo)
//{
//	return true;
//}

ChatGrpcClient::ChatGrpcClient()
{
	//读取对端配置
	auto& cfg = ConfigMgr::Inst();
	auto server_list = cfg["chatservers"]["Name"];

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
