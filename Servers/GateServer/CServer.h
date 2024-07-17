#pragma once

#include"HttpConnection.h"
//��������������ָ��
class CServer :public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& ioc, unsigned short& port);
	void Start();
private:
	net::io_context& _ioc;//�ṩ������
	tcp::acceptor _acceptor;//������
	boost::asio::ip::tcp::socket _socket;//ͨ���׽���
};
