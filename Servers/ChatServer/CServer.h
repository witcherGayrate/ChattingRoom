#pragma once
#include<boost/asio.hpp>
#include"CSession.h"
#include<memory.h>
#include<map>
#include<mutex>
#include<iostream>
#include"AsioIOServicePool.h"
//Cserver�������Ӳ�ά��������Ϣ

using boost::asio::ip::tcp;
class CServer
{
public:
	CServer(boost::asio::io_context& io_context, short port);
	~CServer();
	//ɾ��ĳ������
	void ClearSession(std::string);
private:
	//��������ʱ�Ļص�
	void HandleAccept(std::shared_ptr<CSession>, const boost::system::error_code& error);

	//��ʼ����
	void StartAccept();

	boost::asio::io_context& _io_context;//��clientͨ�ŵ�io_context
	short _port;
	std::map<std::string, std::shared_ptr<CSession>> _sessions;//key:session_id ,shared_ptr<CSession>
	std::mutex _mutex;
	tcp::acceptor _accpetor;

};

