#pragma once
#include"const.h"
#include"Singleton.h"
#include"ConfigMgr.h"
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"message.pb.h"
#include<atomic>
using grpc::Channel;//通信通道
using grpc::Status;//存储结果
using grpc::ClientContext;//通信上下文

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;
using message::LoginRsp;
using message::LoginReq;
using message::StatusService;

//状态服务器连接池供rpc client使用
class StatusConPool
{
public:
	StatusConPool(size_t poolSize, std::string host, std::string port);
	~StatusConPool();
	std::unique_ptr<StatusService::Stub> getConnection();
	void returnConnection(std::unique_ptr<StatusService::Stub> context);
	void Close();

private:
	std::atomic<bool> b_stop_;
	std::queue<std::unique_ptr<StatusService::Stub>> connections_;//连接池队列
	size_t poolSize_;
	std::string port_;
	std::string host_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

class StatusGrpcClient:public Singleton<StatusGrpcClient>
{
	friend class Singleton<StatusGrpcClient>;
public:
	~StatusGrpcClient() {};
	//调用rpc服务的接口
	GetChatServerRsp GetChatServer(int uid);
	LoginRsp Login(int uid, std::string token);
private:
	StatusGrpcClient();
	//单例用unique_ptr管理
	std::unique_ptr<StatusConPool> pool_;

};

