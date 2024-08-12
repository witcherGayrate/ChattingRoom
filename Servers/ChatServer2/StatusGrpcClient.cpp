#include "StatusGrpcClient.h"

StatusConPool::StatusConPool(size_t poolSize, std::string host, std::string port)
	: poolSize_(poolSize), host_(host), port_(port), b_stop_(false)
{
	//��������host��port����rpc����
    for (size_t i = 0; i < poolSize_; ++i) {
        std::shared_ptr<Channel> channel = grpc::CreateChannel(host + ":" + port,
            grpc::InsecureChannelCredentials());
        connections_.push(StatusService::NewStub(channel));
    }
}

StatusConPool::~StatusConPool()
{
    std::lock_guard<std::mutex> lock(mutex_);
    Close();
    while (!connections_.empty()) {
        connections_.pop();
    }
}

std::unique_ptr<StatusService::Stub> StatusConPool::getConnection()
{
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this]() {
        if (b_stop_)
        {
            return true;
        }
        return !connections_.empty();
        });//�ǿջ�ֹͣ�����߳�����ִ��
    //���ֹͣ��ֱ�ӷ��ؿ�ָ��
    if (b_stop_) {
        return  nullptr;
    }
    //��ֹͣ�������ӿ���
    auto context = std::move(connections_.front());//context��unique_ptrû�п�������ֻ��move
    connections_.pop();
    return context;
  
}

void StatusConPool::returnConnection(std::unique_ptr<StatusService::Stub> context)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (b_stop_)
    {
        return;
    }
    connections_.push(std::move(context));
    cond_.notify_one();//����һ��������߳�
}

void StatusConPool::Close()
{
    b_stop_ = true;
    cond_.notify_all();//�������й�����߳�
}

GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
    ClientContext context;
    GetChatServerRsp reply;
    GetChatServerReq request;
    request.set_uid(uid);
    auto stub = pool_->getConnection();
    //�洢���
    Status status = stub->GetChatServer(&context, request, &reply);//GetChatServerԶ�̷�������service
    //�ͻ���ʹ����黹stub
    Defer defer([&stub, this]()
        {
            pool_->returnConnection(std::move(stub));
        });
    //������Ƿ���ȷ
    if (status.ok()) {
        return reply;
    }
    else {
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }

}

LoginRsp StatusGrpcClient::Login(int uid, std::string token)
{
    ClientContext context;
    LoginRsp reply;
    LoginReq request;
    request.set_uid(uid);
    request.set_token(token);

    auto stub = pool_->getConnection();
    Status status = stub->Login(&context, request, &reply);
    //�黹stub
    Defer defer([&stub, this]() {
        pool_->returnConnection(std::move(stub));
        });
    if (status.ok()) {
        return reply;
    }
    else {
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}

StatusGrpcClient::StatusGrpcClient()
{
    auto& gCfgMgr = ConfigMgr::Inst();
    std::string host = gCfgMgr["StatusServer"]["Host"];
    std::string port = gCfgMgr["StatusServer"]["Port"];
    //��ʼ��pool
    pool_.reset(new StatusConPool(5, host, port));
}
