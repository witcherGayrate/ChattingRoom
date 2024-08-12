#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "const.h"
#include"RedisMgr.h"
#include <climits>
std::string generate_unique_string() {
    // ����UUID����
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    // ��UUIDת��Ϊ�ַ���
    std::string unique_string = to_string(uuid);
    return unique_string;
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
    std::string prefix("wechat status server has received :  ");
    std::cout << "wechat status server has received : from gateserver" << std::endl;
    const auto& server = getChatServer();
    reply->set_host(server.host);
    reply->set_port(server.port);
    reply->set_error(ErrorCodes::Success);
    reply->set_token(generate_unique_string());
    insertToken(request->uid(), reply->token());
    return Status::OK;
}

Status StatusServiceImpl::Login(ServerContext* context, const LoginReq* request, LoginRsp* reply)
{
    auto uid = request->uid();
    auto token = request->token();
    
    std::string uid_str = std::to_string(uid);
    std::string token_key = USERTOKENPREFIX + uid_str;
    std::string token_value = "";//����redis��ѯ���
    bool success = RedisMgr::GetInstance()->Get(token_key, token_value);//��ѯ�ɹ�����true��ʧ�ܷ���false
    if (!success)//����Ӧ���ǲ��ɹ�
    {
        reply->set_error(ErrorCodes::UidInvalid);
        return Status::OK;
    }
    if (token_value != token)
    {
        reply->set_error(ErrorCodes::TokenInvalid);
        return Status::OK;
    }

    std::cout << "status server��ѯ��redis��token map" << std::endl;
    reply->set_error(ErrorCodes::Success);
    reply->set_uid(uid);
    reply->set_token(token);
    return Status::OK;

}

void StatusServiceImpl::insertToken(int uid, std::string token)
{
    std::string uid_str = std::to_string(uid);
    std::string token_key = USERTOKENPREFIX + uid_str;
    RedisMgr::GetInstance()->Set(token_key, token);//key:  USERTOKENPREFIX + uid_str;,value:token
}

ChatServer StatusServiceImpl::getChatServer()
{
    //�ȼ���
    std::lock_guard<std::mutex> guard(_server_mtx);
    auto minServer = _servers.begin()->second;
    //ȥredis��ȥ��server������
    auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, minServer.name);
    if (count_str.empty())
    {
        minServer.con_count = INT_MAX;
    }
    else
    {
        minServer.con_count = std::stoi(count_str);//stringתΪint
    }
    //����server ���ҵ���������С��
    for (auto& server : _servers)
    {
        if (server.second.name == minServer.name)
            continue;

        auto count_str = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server.second.name);
        if (count_str.empty())
        {
            server.second.con_count = INT_MAX;
        }
        else
        {
            server.second.con_count = std::stoi(count_str);
        }
        if (server.second.con_count < minServer.con_count)
        {
            minServer = server.second;
        }
    }
    std::cout << "minServer name :" << minServer.name << std::endl;
    return minServer;
}


StatusServiceImpl::StatusServiceImpl()
{
    auto& cfg = ConfigMgr::Inst();
    auto server_list = cfg["chatservers"]["Name"];
    //�ַ�����
    std::stringstream ss(server_list);
    std::vector<std::string> words;
    std::string word;

    while (std::getline(ss,word,','))
    {
        words.push_back(word);
    }
    //��chatserver��Ϣ�洢��map��
    for (auto& word : words)
    {
        if (cfg[word]["Name"].empty())
        {
            continue;
        }

        ChatServer server;
        server.port = cfg[word]["Port"];
        server.host = cfg[word]["Host"];
        server.name = cfg[word]["Name"];
        _servers[server.name] = server;
    }


}
