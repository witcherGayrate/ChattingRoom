#pragma once
#include<boost/asio.hpp>
#include"CSession.h"
#include<memory.h>
#include<map>
#include<mutex>
#include<iostream>
#include"AsioIOServicePool.h"
//Cserver接收连接并维护连接信息

using boost::asio::ip::tcp;
class CServer
{
public:
	CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	//删除某个连接
	void ClearSession(std::string);
private:
	//接收连接时的回调
	void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code& error);

	//开始接收
	void StartAccept();

	boost::asio::io_context& _io_context;//与client通信的io_context
	short _port;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;//key:session_id ,shared_ptr<CSession>
	std::mutex _mutex;
	tcp::acceptor _accpetor;

};

