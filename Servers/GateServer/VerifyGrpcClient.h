#pragma once
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"const.h"
#include"Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;

class RPConPool {
public:
	RPConPool(size_t poolsize, std::string host, std::string port) :
	poolSize_(poolsize), host_(host), port_(port), b_stop_(false) 
	{
		//创建多个rpc通信的channel初始化存根对象stub池
		for (size_t i = 0; i < poolSize_; i++)
		{
			std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port, grpc::InsecureChannelCredentials());
			connections_.push(VerifyService::NewStub(channel));
		}
	}

	void Close()
	{
		b_stop_ = true;
		cond_.notify_all();
	}
	~RPConPool()
	{
		std::lock_guard < std::mutex > lock(mutex_);
		Close();
		while (!connections_.empty())
		{
			connections_.pop();
		}
	}

	std::unique_ptr<VerifyService::Stub> getConnection()
	{
		std::unique_lock<std::mutex> lock(mutex_);
		cond_.wait(lock, [this]() { //返回false进入wait(lock) ，阻塞等待
			if (b_stop_)
			{
				return true; //仍是加锁状态接着往下走
			}
			return !connections_.empty();//return false时解锁，线程挂起不向下执行，等待别的线程唤醒，唤醒后加锁继续往下走
			});
		//如果停止则直接返回空指针
		if (b_stop_)
		{
			return nullptr;
		}
		auto context = std::move(connections_.front());
		connections_.pop();
		return context;

	}

	void returnConnection(std::unique_ptr<VerifyService::Stub> context)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		if (b_stop_)
		{
			return;
		}
		connections_.push(std::move(context));//归还
		cond_.notify_one();//归还一个stub之后，唤醒一个等待使用stub的线程
	}

private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VerifyService::Stub>> connections_;
	std::mutex mutex_;
	std::condition_variable cond_;
};


class VerifyGrpcClient:public Singleton<VerifyGrpcClient>
{
	friend class Singleton<VerifyGrpcClient>;
public:
	GetVerifyRsp GetVerifyCode(std::string email)
	{
		ClientContext context;
		GetVerifyReq request;
		GetVerifyRsp reply;
		request.set_email(email);
		//从grpcPool获取一个stub
		auto stub = pool_->getConnection();
		Status status = stub->GetVerifyCode(& context, request, &reply);
		if (status.ok())
		{
			//用完stub归还
			pool_->returnConnection(std::move(stub));
			return reply;
		}
		else {
			pool_->returnConnection(std::move(stub));
			reply.set_error(ErrorCodes::RPCFailed);
			return reply;
		}
	}
private:
	VerifyGrpcClient();
		//std::shared_ptr<channel> channel = grpc::createchannel("0.0.0.0:50051", 
		//	grpc::insecurechannelcredentials());
		//stub_ = verifyservice::newstub(channel);
	
	//std::unique_ptr<VerifyService::Stub> stub_;//用stub池子
	std::unique_ptr<RPConPool> pool_;
};

