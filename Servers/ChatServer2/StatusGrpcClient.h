#pragma once
#include"const.h"
#include"Singleton.h"
#include"ConfigMgr.h"
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"message.pb.h"
#include<atomic>
using grpc::Channel;//ͨ��ͨ��
using grpc::Status;//�洢���
using grpc::ClientContext;//ͨ��������

using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::StatusService;
using message::LoginRsp;
using message::LoginReq;
using message::StatusService;

//״̬���������ӳع�rpc clientʹ��
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
	std::queue<std::unique_ptr<StatusService::Stub>> connections_;//���ӳض���
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
	//����rpc����Ľӿ�
	GetChatServerRsp GetChatServer(int uid);
	LoginRsp Login(int uid, std::string token);
private:
	StatusGrpcClient();
	//������unique_ptr����
	std::unique_ptr<StatusConPool> pool_;

};

