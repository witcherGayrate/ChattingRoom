#pragma once
#include"Singleton.h"
#include<functional>
#include<map>
#include"const.h"

class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;//存储httpconnection的回调函数对象

class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem() {};//智能指针可以调用
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);//处理get请求类型的回调
	bool HandlePost(std::string path, std::shared_ptr<HttpConnection> con);
	void RegGet(std::string, HttpHandler handler);//实现消息注册 ，RegGet函数，接受路由和回调函数作为参数
	void RegPost(std::string, HttpHandler handler);//实现post的handle
private:
	LogicSystem();
	std::map < std::string, HttpHandler > _post_handlers; //post请求对应的函数表,key为路由，value为回调函数
	std::map<std::string, HttpHandler> _get_handlers;//get请求对应的函数表,key为路由，value为回调函数
};

