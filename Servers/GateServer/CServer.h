#pragma once

#include"HttpConnection.h"
//分享自身的智能指针
class CServer :public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	void Start();
private:
	net::io_context& _ioc;//提供上下文
	tcp::acceptor _acceptor;//接收器
	boost::asio::ip::tcp::socket _socket;//通信套接字
};

