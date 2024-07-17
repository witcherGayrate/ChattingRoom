#include "HttpConnection.h"
#include"LogicSystem.h"

//解析url用到的全局函数
//char 转为16进制
unsigned char ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if (x >= 'A' && x <= 'Z')y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z')y = x - 'a' + 10;
	else if (x >= '0' && x <= '9')y = x - '0';
	else assert(0);//断言条件为假则终止程序
	return y;
}

std::string UrlEncode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//判断是否仅有数字和字母构成
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ') //为空字符
			strTemp += "+";
		else
		{
			//其他字符需要提前加%并且高四位和低四位分别转为16进制
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4); //Od:0000 0000
			strTemp += ToHex((unsigned char)str[i] & 0x0F); //0000 1111
		}
	}
	return strTemp;
}

std::string UrlDecode(const std::string& str)
{
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++)
	{
		//还原+为空
		if (str[i] == '+') strTemp += ' ';
		//遇到%将后面的两个字符从16进制转为char再拼接
		else if (str[i] == '%')
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		}
		else strTemp += str[i];
	}
	return strTemp;
}

//HttpConnection的实现
HttpConnection::HttpConnection(boost::asio::io_context& ioc)
:_socket(ioc)//接过新连接的_cocket所有权
{
	//_socket用ioc上下文初始化
}

void HttpConnection::Start()
{
	auto self = shared_from_this();
	http::async_read(_socket, _buffer, _request, [self](beast::error_code ec, std::size_t bytes_transferred) {
		try
		{
			if (ec)
			{
				std::cout << "http read err is " << ec.what() << std::endl;
				return;
			}

			//处理读到的数据
			boost::ignore_unused(bytes_transferred);

			self->HandleReq();

			self->CheckDeadline();
		}
		catch (const std::exception& exp)
		{
			std::cout << "exception is " << exp.what() << std::endl;
		}

		});
}

tcp::socket& HttpConnection::GetSocket()
{
	return _socket;
}

//发送数据包不能超时，在发送时我们启动一个定时器，收到发送的回调后取消定时器。
void HttpConnection::CheckDeadline()
{
	auto self = shared_from_this();
	deadline_.async_wait([self](beast::error_code ec)
		{
			if (!ec)
			{
				//Close socket to cancel any outstanding operation.
				self->_socket.close();
			}
		});
}

void HttpConnection::WriteResponse()
{
	auto self = shared_from_this();
	_response.content_length(_response.body().size());//设置response的content length
	http::async_write(_socket,
		_response,
		[self](beast::error_code ec, std::size_t)
		{
			//关闭server的send
			self->_socket.shutdown(tcp::socket::shutdown_send, ec);
			self->deadline_.cancel();
		});
}

//成功读取request之后进行处理
void HttpConnection::HandleReq()
{
	//设置版本
	_response.version(_request.version());
	//设置为短连接
	_response.keep_alive(false);

	//请求头为get请求时
	if (_request.method() == http::verb::get)
	{
		//解析request的url
		PreParseGetParam();
		//将请求target和查询参数分离，传参给logicsystem从map中调用对应的回调函数
		bool success = LogicSystem::GetInstance()->HandleGet(_get_url, shared_from_this());
		
		//把get请求的处理交给logicsystem单例的handleget方法,返回值是bool
		if (!success)
		{
			_response.result(http::status::not_found);//设置回应头的result字段
			_response.set(http::field::content_type, "text/plain");//context_type字段
			beast::ostream(_response.body()) << "url not found\r\n";//错误信息写入response的消息体
			WriteResponse();
			return;

		}
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}

	if (_request.method() == http::verb::post) {
		bool success = LogicSystem::GetInstance()->HandlePost(_request.target(), shared_from_this());
		if (!success)
		{
			_response.result(http::status::not_found);
			_response.set(http::field::content_type, "text/plain");
			beast::ostream(_response.body()) << "url not found\r\n";
			WriteResponse();
			return;
		}
		_response.result(http::status::ok);
		_response.set(http::field::server, "GateServer");
		WriteResponse();
		return;
	}
}

//参数解析
void HttpConnection::PreParseGetParam()
{
	//提取url
	auto url = _request.target();
	//查找查询字符串的开始位置（即？的位置）
	auto query_pos = url.find('?');
	if (query_pos == std::string::npos)
	{
		_get_url = url;
		return;
	}

	_get_url = url.substr(0, query_pos);//保留？出现之前的字符
	std::string query_string = url.substr(query_pos + 1);
	std::string key, value;
	size_t pos = 0;
	while ((pos=query_string.find('&'))!= std::string::npos )
	{
		auto pair = query_string.substr(0, pos);//初始化pair
		size_t eq_pos = pair.find('=');
		if (eq_pos != std::string::npos)
		{
			key = UrlDecode(pair.substr(0, eq_pos));
			value = UrlDecode(pair.substr(eq_pos + 1));
			_get_params[key] = value;
		}
		query_string.erase(0, pos + 1);
	}
	//处理最后一个参数对(如果没有&分隔符)
	if (!query_string.empty())
	{
		size_t eq_pos = query_string.find('=');
		if (eq_pos != std::string::npos)
		{
			key = UrlDecode(query_string.substr(0, eq_pos));
			value = UrlDecode(query_string.substr(eq_pos + 1));
			_get_params[key] = value;
		}
	}
}


