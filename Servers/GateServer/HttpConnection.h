#pragma once
#include"const.h"

class HttpConnection:public std::enable_shared_from_this<HttpConnection>
{
	friend class LogicSystem;//逻辑处理类
public:
	
	HttpConnection(boost::asio::io_context &ioc);//socket to peer
	void Start();//异步读
	tcp::socket& GetSocket();//返回套接字
private:
	void CheckDeadline();
	void WriteResponse();//写response
	void HandleReq();//请求回调
	void PreParseGetParam();
	tcp::socket _socket;//通信with peer的套接字

	//the buffer for performing reads
	beast::flat_buffer _buffer{ 8192 };//ATM 的常用MTU

	//the request message
	http::request<http::dynamic_body> _request;//可变消息体类型

	//the response message
	http::response<http::dynamic_body> _response;

	//the timer for putting a deadling on connection processing
	net::steady_timer deadline_
	{
		//initialize with executor and duration
		_socket.get_executor(),std::chrono::seconds(60)//60s connection life
	};

	//get_url目标
	std::string _get_url;
	std::unordered_map<std::string, std::string> _get_params;//识别的键值对

};

