#include "ChatGrpcClient.h"

ChatConPool::ChatConPool(int pool_size, std::string host, std::string port) :
	_pool_size(pool_size), _host(host), _port(port), _b_stop(false)
{
	//��ʼ��rpc���ӳ�
	for (int i = 0; i < _pool_size; i++)
	{
		//����channel
		std::shared_ptr<Channel> channel = grpc::CreateChannel(_host + ":" + _port, grpc::InsecureChannelCredentials());//����һ�����ð�ȫ��֤��channel
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
	_cond.wait(lock, [this]()
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
	//��ȡ�Զ�����
	auto& cfg = ConfigMgr::Inst();
	auto server_list = cfg["chatservers"]["Name"];

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
