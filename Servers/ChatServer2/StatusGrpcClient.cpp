#include "StatusGrpcClient.h"

StatusConPool::StatusConPool(size_t poolSize, std::string host, std::string port)
	: poolSize_(poolSize), host_(host), port_(port), b_stop_(false)
{
	//创建根据host和port创建rpc连接
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
        });//非空或停止则唤醒线程向下执行
    //如果停止则直接返回空指针
    if (b_stop_) {
        return  nullptr;
    }
    //不停止且有连接可用
    auto context = std::move(connections_.front());//context是unique_ptr没有拷贝构造只能move
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
    cond_.notify_one();//唤醒一个挂起的线程
}

void StatusConPool::Close()
{
    b_stop_ = true;
    cond_.notify_all();//唤醒所有挂起的线程
}

GetChatServerRsp StatusGrpcClient::GetChatServer(int uid)
{
    ClientContext context;
    GetChatServerRsp reply;
    GetChatServerReq request;
    request.set_uid(uid);
    auto stub = pool_->getConnection();
    //存储结果
    Status status = stub->GetChatServer(&context, request, &reply);//GetChatServer远程服务器的service
    //客户端使用完归还stub
    Defer defer([&stub, this]()
        {
            pool_->returnConnection(std::move(stub));
        });
    //检查结果是否正确
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
    //归还stub
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
    //初始化pool
    pool_.reset(new StatusConPool(5, host, port));
}
