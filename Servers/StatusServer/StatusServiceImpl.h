#pragma once
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using message::GetChatServerReq;
using message::GetChatServerRsp;
using message::LoginReq;
using message::LoginRsp;
using message::StatusService;


struct ChatServer {
    std::string host;
    std::string port;
    std::string name;
    int con_count;
};//消息体

//终极继承自StatusService::Service
class StatusServiceImpl final : public StatusService::Service
{
public:
    StatusServiceImpl();
    
    Status GetChatServer(ServerContext* context, const GetChatServerReq* request,
        GetChatServerRsp* reply) override;//重写rpc服务中的getChatServer service
    Status Login(ServerContext* context, const LoginReq* request,
        LoginRsp* reply) override;
private:
    void insertToken(int uid, std::string token);//维护user_token
    //根据负载量选择chatserver
    ChatServer getChatServer();
    std::unordered_map<std::string, ChatServer> _servers;//servers map
    std::mutex _server_mtx;


};