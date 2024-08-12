#pragma once
#include"const.h"
#include"ConfigMgr.h"
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"message.pb.h"


using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::AddFriendReq;
using message::AddFriendRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::GetChatServerRsp;
using message::LoginReq;
using message::LoginRsp;
using message::ChatService;

using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;

class ChatConPool
{
public:
	ChatConPool(int pool_size, std::string host, std::string port);
	~ChatConPool();

	std::unique_ptr<ChatService::Stub> getConnection();
	void returnConnection(std::unique_ptr<ChatService::Stub>);
	void Close();
private:
	std::atomic<bool> _b_stop;//连接池是否关闭
	std::mutex _mutex;
	std::condition_variable _cond;
	std::queue<std::unique_ptr<ChatService::Stub>> _connections;
	std::string _host;
	std::string _port;
	size_t _pool_size;

};
//grpc客户端
class ChatGrpcClient :public Singleton<ChatGrpcClient>
{
	friend class Singleton<ChatGrpcClient>;
public:

	~ChatGrpcClient() {};
	AddFriendRsp NotifyAddFriend(std::string server_ip, const AddFriendReq& req);
	//AuthFriendRsp NotifyAuthFriend(std::string server_ip, const AuthFriendReq& req);//ip表示peer server
	// NotifyTextChatMsg(std::string server_ip, const TextChatMsgReq& req, const Json::Value& rtValue);
	//bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);//query usreinfo by key
private:
	ChatGrpcClient();
	std::unordered_map<std::string, std::unique_ptr<ChatConPool>> _pools;//与每个chatserver通信的rpc连接池

};

