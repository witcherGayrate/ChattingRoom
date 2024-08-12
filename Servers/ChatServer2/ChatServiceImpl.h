#pragma once
#include<grpcpp/grpcpp.h>
#include"message.grpc.pb.h"
#include"message.pb.h"
#include"data.h"
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using message::AddFriendReq;
using message::AddFriendRsp;

using message::AuthFriendReq;
using message::AuthFriendRsp;

using message::GetChatServerRsp;
using message::LoginReq;
using message::LoginRsp;
using message::ChatService;

using message::RplyFriendReq;
using message::RplyFriendRsp;
using message::SendChatMsgReq;
using message::SendChatMsgRsp;

using message::TextChatMsgReq;
using message::TextChatMsgRsp;
using message::TextChatData;

class ChatServiceImpl final :public ChatService::Service
{
public:
	ChatServiceImpl();
    //ÖØÐ´ChatService::ServiceµÄÐéº¯Êý
    virtual Status NotifyAddFriend(ServerContext* context, const AddFriendReq* request, AddFriendRsp* response) override;
    //virtual Status RplyAddFriend(ServerContext* context, const RplyFriendReq* request, RplyFriendRsp* response) override;
   //virtual Status SendChatMsg(ServerContext* context, const SendChatMsgReq* request, SendChatMsgRsp* response) override;
    virtual Status NotifyAuthFriend(ServerContext* context, const AuthFriendReq* request, AuthFriendRsp* response) override;
    virtual Status NotifyTextChatMsg(ServerContext* context, const TextChatMsgReq* request, TextChatMsgRsp* response)override;

    bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
};

