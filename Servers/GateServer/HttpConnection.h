#pragma once
#include"const.h"

class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;//�߼�������
public:
	
	HttpConnection(boost::asio::io_context &ioc);//socket to peer
	void Start();//�첽��
	tcp::socket& GetSocket();//�����׽���
private:
	void CheckDeadline();
	void WriteResponse();//дresponse
	void HandleReq();//����ص�
	void PreParseGetParam();
	tcp::socket _socket;//ͨ��with peer���׽���

	//the buffer for performing reads
	beast::flat_buffer _buffer{ 8192 };//ATM �ĳ���MTU

	//the request message
	http::request<http::dynamic_body> _request;//�ɱ���Ϣ������

	//the response message
	http::response<http::dynamic_body> _response;

	//the timer for putting a deadling on connection processing
	net::steady_timer deadline_
	{
		//initialize with executor and duration
		_socket.get_executor(),std::chrono::seconds(60)//60s connection life
	};

	//get_urlĿ��
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;//ʶ��ļ�ֵ��

};

