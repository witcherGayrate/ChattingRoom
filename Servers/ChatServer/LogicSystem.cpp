#include "LogicSystem.h"
#include"StatusGrpcClient.h"
#include"MysqlMgr.h"
#include"RedisMgr.h"
#include"UserMgr.h"
#include"ChatGrpcClient.h"
LogicSystem::~LogicSystem()
{
	_b_stop = true;
	//唤醒挂起的线程
	_consume.notify_one();
	_worker_thread.join();
}

void LogicSystem::PostMsgToQue(shared_ptr<LogicNode> msg)
{
	std::unique_lock<std::mutex> unique_lk(_mutex);
	_msg_que.push(msg);
	//由0变1则发送通知信号
	if (_msg_que.size() == 1)
	{
		unique_lk.unlock();
		_consume.notify_one();
	}
}

bool LogicSystem::isPureDigit(const std::string& str)
{
	for (char c : str)
	{
		//判断字符是否是数字
		if (!std::isdigit(c))
		{
			return false;
		}
	}
	return true;
}

LogicSystem::LogicSystem():_b_stop(false)
{
	RegisterCallBacks();
	_worker_thread = std::thread(&LogicSystem::DealMsg, this);
}

void LogicSystem::DealMsg()
{
	for (;;)
	{
		std::unique_lock<std::mutex> unique_lk(_mutex);
		//判断队列为空则用条件变量阻塞等待，并释放锁
		while (_msg_que.empty() && !_b_stop)
		{
			_consume.wait(unique_lk);
		}
		//判断是否为关闭状态，把所有逻辑执行完后则退出循环
		if (_b_stop)
		{
			while (!_msg_que.empty())
			{
				auto msg_node = _msg_que.front();
				cout << "recv_msg is " << msg_node->_recvnode->_msg_id << endl;
				//调用相应的回调
				auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
				if (call_back_iter == _fun_callbacks.end())
				{
					_msg_que.pop();
					continue;
				}
				call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
					std::string(msg_node->_recvnode->_data,msg_node->_recvnode->_cur_len));
				_msg_que.pop();
			}
			break;
		}
		//如果没有停服，说明队列里还有数据，需要处理
		auto msg_node = _msg_que.front();
		cout << "recv_msg id  is " << msg_node->_recvnode->_msg_id << endl;
		auto call_back_iter = _fun_callbacks.find(msg_node->_recvnode->_msg_id);
		if (call_back_iter == _fun_callbacks.end()) {
			_msg_que.pop();
			std::cout << "msg id [" << msg_node->_recvnode->_msg_id << "] handler not found" << std::endl;
			continue;
		}
		call_back_iter->second(msg_node->_session, msg_node->_recvnode->_msg_id,
			std::string(msg_node->_recvnode->_data, msg_node->_recvnode->_cur_len));
		_msg_que.pop();
	}
}

void LogicSystem::RegisterCallBacks()
{
	//注册登录请求的回调
	_fun_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this, std::placeholders::_1,
		placeholders::_2, placeholders::_3);
	//注册搜索用户请求的回调
	_fun_callbacks[ID_SEARCH_USER_REQ] = std::bind(&LogicSystem::SearchInfo, this, std::placeholders::_1,
		placeholders::_2, placeholders::_3);
	//注册申请好友请求(ID_ADD_FRIEND_REQ)的回调
	_fun_callbacks[ID_ADD_FRIEND_REQ] = std::bind(&LogicSystem::AddFriendApply, this, std::placeholders::_1,
		placeholders::_2, placeholders::_3);
	//注册好友认证的请求
	_fun_callbacks[ID_AUTH_FRIEND_REQ] = std::bind(&LogicSystem::AuthFriendApply, this, std::placeholders::_1,
		placeholders::_2, placeholders::_3);
}

void LogicSystem::LoginHandler(shared_ptr<CSession>session, const short& msg_id, const string& msg_data)
{
	//解析msg_data
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	auto uid = root["uid"].asInt();
	auto token = root["token"].asString();
	std::cout << "user login uid is  " << uid << " user token  is "
		<< token << endl;

	//从redis获取token匹配是否正确
	Json::Value rtvalue;
	//最后发送回包
	Defer defer([this, &rtvalue, session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, MSG_CHAT_LOGIN_RSP);
		});
	
	std::string uid_str = std::to_string(uid);
	std::string token_key = USERTOKENPREFIX + uid_str;
	std::string token_value = "";
	bool success = RedisMgr::GetInstance()->Get(token_key, token_value);
	std::cout << "ChatServer query token of uid from redis: " << token_value << std::endl;
	if (!success)
	{
		rtvalue["error"] = ErrorCodes::UidInvalid;
		return;
	}
	if (token_value != token)//user传来的token跟redis中的不一致
	{
		rtvalue["error"] = ErrorCodes::TokenInvalid;
		return;
	}
	rtvalue["error"] = ErrorCodes::Success;

	//用户的token对应上之后，去redis中查询用户信息
	std::string base_key = USER_BASE_INFO + uid_str;
	auto user_info = std::make_shared<UserInfo>();
	bool b_base = GetBaseInfo(base_key, uid, user_info);
	if (!b_base)
	{
		rtvalue["error"] = ErrorCodes::UidInvalid;
		return;
	}

	rtvalue["uid"] = uid;
	rtvalue["pwd"] = user_info->pwd;
	rtvalue["name"] = user_info->name;
	rtvalue["email"] = user_info->email;
	rtvalue["nick"] = user_info->nick;
	rtvalue["desc"] = user_info->desc;
	rtvalue["sex"] = user_info->sex;
	rtvalue["icon"] = user_info->icon;

	//debug日志
	//std::cout << "Chatserver::loginhandler query userbaseinfo from mysql  rtvalue[icon] : " << user_info->icon << std::endl;
	
	//从数据库获取好友申请
	std::vector<std::shared_ptr<ApplyInfo>> apply_list;
	auto b_apply = GetFriendApplyInfo(uid, apply_list);
	if (b_apply)
	{
		for (auto& apply : apply_list)
		{
			Json::Value obj;
			obj["name"] = apply->_name;
			obj["uid"] = apply->_uid;
			obj["icon"] = apply->_icon;
			obj["nick"] = apply->_nick;
			obj["sex"] = apply->_sex;
			obj["desc"] = apply->_desc;
			obj["status"] = apply->_status;
			rtvalue["apply_list"].append(obj);
			//std::cout << "ChatServer GetFriendApplyInfo :icon " << apply->_icon << std::endl;;

		}
		
	}
	//获取好友列表

	std::vector<std::shared_ptr<UserInfo>> friend_list;
	bool b_friend_list = GetFriendList(uid, friend_list);
	for (auto& friend_ele : friend_list) {
		Json::Value obj;
		obj["name"] = friend_ele->name;
		obj["uid"] = friend_ele->uid;
		obj["icon"] = friend_ele->icon;
		obj["nick"] = friend_ele->nick;
		obj["sex"] = friend_ele->sex;
		obj["desc"] = friend_ele->desc;
		obj["back"] = friend_ele->back;
		rtvalue["friend_list"].append(obj);
	}

	//将登录数量加一
	auto server_name = ConfigMgr::Inst().GetValue("SelfServer", "Name");
	auto rd_res = RedisMgr::GetInstance()->HGet(LOGIN_COUNT, server_name);
	int count = 0;
	if (!rd_res.empty())
	{
		count = std::stoi(rd_res);
	}
	count++;
	//把连接数写入redis
	auto count_str = std::to_string(count);//字符串存储
	RedisMgr::GetInstance()->HSet(LOGIN_COUNT, server_name, count_str);

	//绑定session和uid
	session->SetUserId(uid);

	//为用户设置登录ip server的名字
	std::string ipkey = USERIPPREFIX + uid_str;
	RedisMgr::GetInstance()->Set(ipkey, server_name);//user登录所在chatserver

	//uid和session绑定管理，方便踢人操作
	UserMgr::GetInstance()->SetUserSession(uid, session);
	

	std::cout << "has achieved to return" << std::endl;
	return;
}

void LogicSystem::SearchInfo(shared_ptr<CSession> session, const short& msg_id, const string& msg_data)
{
	//解析收到的字符序列
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	auto uid_str = root["uid"].asString();
	std::cout << "user SearchInfo uid is " << uid_str << std::endl;

	Json::Value rtValue;
	//调用结束时发送回包至消息队列（1.削峰2.多线程安全3.保证顺序）
	Defer defer([this, &rtValue, session]()
		{
			std::string return_str = rtValue.toStyledString();
			session->Send(return_str, ID_SEARCH_USER_RSP);
		});

	bool b_digit = isPureDigit(uid_str);
	if (b_digit)
	{
		GetUserByUid(uid_str, rtValue);
	}
	else
	{
		GetUserByName(uid_str, rtValue);
	}
}
void LogicSystem::GetUserByUid(const std::string uid_str, Json::Value& rtValue)
{
	rtValue["error"] = ErrorCodes::Success;
	std::string base_key = USER_BASE_INFO + uid_str;
	//优先查询redis中的用户信息
	std::string info_str = "";
	bool b_base = RedisMgr::GetInstance()->Get(base_key, info_str);
	if (b_base)
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		auto uid = root["uid"].asInt();
		auto name = root["name"].asString();
		auto pwd = root["pwd"].asString();
		auto email = root["email"].asString();
		auto nick = root["nick"].asString();
		auto desc = root["desc"].asString();
		auto sex = root["sex"].asInt();
		auto icon = root["icon"].asString();
		std::cout << "user uid is " << uid << " name is " << name << " pwd is "
			<< pwd << " email is " << email << " icon is" << icon << endl;

		rtValue["uid"] = uid;
		rtValue["pwd"] = pwd;
		rtValue["name"] = name;
		rtValue["email"] = email;
		rtValue["nick"] = nick;
		rtValue["desc"] = desc;
		rtValue["sex"] = sex;
		rtValue["icon"] = icon;
		return;
	}
	//redis中没有则查询mysql
	auto uid = std::stoi(uid_str);
	//查询数据库
	std::shared_ptr<UserInfo> user_info = nullptr;
	user_info = MysqlMgr::GetInstance()->GetUser(uid);
	if (user_info == nullptr)
	{
		rtValue["error"] = ErrorCodes::UidInvalid;
		return;
	}
	//将从数据库中查到的内容写入redis缓存
	Json::Value redis_root;
	redis_root["uid"] = user_info->uid;
	redis_root["pwd"] = user_info->pwd;
	redis_root["name"] = user_info->name;
	redis_root["email"] = user_info->email;
	redis_root["nick"] = user_info->nick;
	redis_root["desc"] = user_info->desc;
	redis_root["sex"] = user_info->sex;
	redis_root["icon"] = user_info->icon;

	RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());
	//返回数据
	rtValue["uid"] = user_info->uid;
	rtValue["pwd"] = user_info->pwd;
	rtValue["name"] = user_info->name;
	rtValue["email"] = user_info->email;
	rtValue["nick"] = user_info->nick;
	rtValue["desc"] = user_info->desc;
	rtValue["sex"] = user_info->sex;
	rtValue["icon"] = user_info->icon;

}

void LogicSystem::GetUserByName(const std::string name, Json::Value& rtValue)
{
	rtValue["error"] = ErrorCodes::Success;
	std::string base_key = NAME_INFO + name;
	//优先查询redis中的用户信息
	std::string info_str = "";
	bool b_base = RedisMgr::GetInstance()->Get(base_key, info_str);
	if (b_base)
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		auto uid = root["uid"].asInt();
		auto name = root["name"].asString();
		auto pwd = root["pwd"].asString();
		auto email = root["email"].asString();
		auto nick = root["nick"].asString();
		auto desc = root["desc"].asString();
		auto sex = root["sex"].asInt();
		auto icon = root["icon"].asString();
		std::cout << "user uid is " << uid << " name is " << name << " pwd is "
			<< pwd << " email is " << email << " icon is" << icon << endl;

		rtValue["uid"] = uid;
		rtValue["pwd"] = pwd;
		rtValue["name"] = name;
		rtValue["email"] = email;
		rtValue["nick"] = nick;
		rtValue["desc"] = desc;
		rtValue["sex"] = sex;
		//rtValue["icon"] = icon;
		return;
	}
	//redis中没有则查询mysql
	//查询数据库
	std::shared_ptr<UserInfo> user_info = nullptr;
	user_info = MysqlMgr::GetInstance()->GetUser(name);
	if (user_info == nullptr)
	{
		rtValue["error"] = ErrorCodes::UidInvalid;
		return;
	}
	//将从数据库中查到的内容写入redis缓存
	Json::Value redis_root;
	redis_root["uid"] = user_info->uid;
	redis_root["pwd"] = user_info->pwd;
	redis_root["name"] = user_info->name;
	redis_root["email"] = user_info->email;
	redis_root["nick"] = user_info->nick;
	redis_root["desc"] = user_info->desc;
	redis_root["sex"] = user_info->sex;
	//redis_root["icon"] = user_info->icon;
	
	RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());
	//返回数据
	rtValue["uid"] = user_info->uid;
	rtValue["pwd"] = user_info->pwd;
	rtValue["name"] = user_info->name;
	rtValue["email"] = user_info->email;
	rtValue["nick"] = user_info->nick;
	rtValue["desc"] = user_info->desc;
	rtValue["sex"] = user_info->sex;
	//rtValue["icon"] = user_info->icon;
}

bool LogicSystem::GetFriendApplyInfo(const int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& list)
{
	return MysqlMgr::GetInstance()->GetApplyList(to_uid,list,0,10);
}

void LogicSystem::AddFriendApply(std::shared_ptr<CSession> session, const short& msg_id, const string& msg_data)
{
	//解析req包的数据字段
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	auto uid = root["uid"].asInt();
	auto applyname = root["applyname"].asString();
	auto bakname = root["bakname"].asString();
	auto touid = root["touid"].asInt();
	std::cout << "user applyfriend uid is  " << uid << " applyname  is "
		<< applyname << " bakname is " << bakname << " touid is " << touid << endl;

	Json::Value  rtvalue;
	rtvalue["error"] = ErrorCodes::Success;
	//回包加入send queue
	Defer defer([this, &rtvalue, session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, ID_ADD_FRIEND_RSP);
		});
	//更新数据库
	MysqlMgr::GetInstance()->AddFriendApply(uid, touid);
	//查询redis，查找touid对应的server ip(对方在线的话就会在redis中注册登录信息)
	auto to_str = std::to_string(touid);
	auto to_ip_key = USERIPPREFIX + to_str;
	std::string to_ip_value = "";
	bool b_ip = RedisMgr::GetInstance()->Get(to_ip_key, to_ip_value);
	if (!b_ip)
	{
		return;
	}

	auto& cfg = ConfigMgr::Inst();
	auto self_name = cfg["SelfServer"]["Name"];

	//如果被申请方与申请方不在同一个服务器,则把自己的信息给对方所在chatserver发送过去
	std::string base_key = USER_BASE_INFO + std::to_string(uid);
	auto apply_info = std::make_shared<UserInfo>();
	bool b_info = GetBaseInfo(base_key, uid, apply_info);

	//如果被申请方也在同一个chatserver服务器那么直接找到它的连接session发送申请者的请求
	if (to_ip_value == self_name)
	{
		auto session = UserMgr::GetInstance()->GetSession(touid);
		if (session)
		{
			//通过session直接发通知给对方
			Json::Value notify;
			notify["error"] = ErrorCodes::Success;
			notify["applyuid"] = uid;
			notify["name"] = applyname;
			notify["desc"] = "";
			if (b_info)
			{
				notify["icon"] = apply_info->icon;
				notify["sex"] = apply_info->sex;
				notify["nick"] = apply_info->nick;
			}
			std::string return_str = notify.toStyledString();
			session->Send(return_str, ID_NOTIFY_ADD_FRIEND_REQ);
		}
		return;
	}


	AddFriendReq add_req;//rpc 格式message
	add_req.set_applyuid(uid);
	add_req.set_touid(touid);
	add_req.set_name(applyname);
	add_req.set_desc("");
	if (b_info)
	{
		add_req.set_icon(apply_info->icon);
		add_req.set_sex(apply_info->sex);
		add_req.set_nick(apply_info->nick);
	}
	//调用rpc发送通知
	ChatGrpcClient::GetInstance()->NotifyAddFriend(to_ip_value, add_req);


}

void LogicSystem::AuthFriendApply(std::shared_ptr<CSession> session, const short& msg_id, const string& msg_data)
{
	
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);

	auto uid = root["fromuid"].asInt();
	auto touid = root["touid"].asInt();
	auto back_name = root["back"].asString();
	std::cout << "from " << uid << " auth friend to " << touid << std::endl;

	Json::Value  rtvalue;
	rtvalue["error"] = ErrorCodes::Success;
	auto user_info = std::make_shared<UserInfo>();

	//返回消息给认证方
	std::string base_key = USER_BASE_INFO + std::to_string(touid);
	bool b_info = GetBaseInfo(base_key, touid, user_info);
	if (b_info) {
		rtvalue["name"] = user_info->name;
		rtvalue["nick"] = user_info->nick;
		rtvalue["icon"] = user_info->icon;
		rtvalue["sex"] = user_info->sex;
		rtvalue["uid"] = touid;
	}
	else {
		rtvalue["error"] = ErrorCodes::UidInvalid;
	}


	Defer defer([this, &rtvalue, session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, ID_AUTH_FRIEND_RSP);
		});

	//先更新数据库
	bool  apply_status = MysqlMgr::GetInstance()->AuthFriendApply(uid, touid);
	if (!apply_status)
	{
		std::cout << "ChatServer failed to set the status of friend apply from 0 to 1 " << std::endl;
	}

	//更新数据库添加好友
	bool friend_table =  MysqlMgr::GetInstance()->AddFriend(uid, touid, back_name);
	if (!friend_table)
	{
		std::cout << "ChatServer failed to set the add friend info for fromuid and touid " << std::endl;
	}

	//查询redis 查找touid对应的server ip
	auto to_str = std::to_string(touid);
	auto to_ip_key = USERIPPREFIX + to_str;
	std::string to_ip_value = "";
	bool b_ip = RedisMgr::GetInstance()->Get(to_ip_key, to_ip_value);
	if (!b_ip) {
		return;
	}

	auto& cfg = ConfigMgr::Inst();
	auto self_name = cfg["SelfServer"]["Name"];

	//直接通知申请方有认证通过消息，需判断是否在本服务
	if (to_ip_value == self_name) {
		auto session = UserMgr::GetInstance()->GetSession(touid);
		if (session) {
			//在内存中则直接发送通知对方
			Json::Value  notify;
			notify["error"] = ErrorCodes::Success;
			notify["fromuid"] = uid;
			notify["touid"] = touid;
			std::string base_key = USER_BASE_INFO + std::to_string(uid);
			auto user_info = std::make_shared<UserInfo>();
			bool b_info = GetBaseInfo(base_key, uid, user_info);
			if (b_info) {
				notify["name"] = user_info->name;
				notify["nick"] = user_info->nick;
				notify["icon"] = user_info->icon;
				notify["sex"] = user_info->sex;
			}
			else {
				notify["error"] = ErrorCodes::UidInvalid;
			}


			std::string return_str = notify.toStyledString();
			session->Send(return_str, ID_NOTIFY_AUTH_FRIEND_REQ);
		}

		return;
	}


	AuthFriendReq auth_req;
	auth_req.set_fromuid(uid);
	auth_req.set_touid(touid);

	//rpcClient调用rpc服务发送通知
	ChatGrpcClient::GetInstance()->NotifyAuthFriend(to_ip_value, auth_req);

}


bool LogicSystem::GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo)
{
	//从redis中查询用户信息
	std::string info_str = "";
	bool b_base = RedisMgr::GetInstance()->Get(base_key, info_str);
	//debug 没有在redis中找到base_key，对应的信息,redis数据可能是旧的，先从mysql中查询
	if (b_base)
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(info_str, root);
		userinfo->uid = root["uid"].asInt();
		userinfo->name = root["name"].asString();
		userinfo->pwd = root["pwd"].asString();
		userinfo->email = root["email"].asString();
		userinfo->nick = root["nick"].asString();
		userinfo->desc = root["desc"].asString();
		userinfo->sex = root["sex"].asInt();
		userinfo->icon = root["icon"].asString();
		/*std::cout << "user login uid is " << userinfo->uid << " name is " <<
			userinfo->name << " pwd is " << userinfo->pwd << " email is " << userinfo->email << 
			" icon is "<<userinfo->icon<<std::endl;*/
		if (root["icon"].asString() == "")//初次登录redis中没有完整用户信息
		{
			//查询数据库
			std::shared_ptr<UserInfo> user_info = nullptr;
			user_info = MysqlMgr::GetInstance()->GetUser(uid);
			if (user_info == nullptr)
			{
				return false;
			}

			userinfo = user_info;//更新接收数据对象userinfo
			//将数据内容写入redis缓存
			Json::Value redis_root;
			redis_root["uid"] = uid;
			redis_root["pwd"] = userinfo->pwd;
			redis_root["name"] = userinfo->name;
			redis_root["nick"] = userinfo->nick;
			redis_root["email"] = userinfo->email;
			redis_root["desc"] = userinfo->desc;
			redis_root["sex"] = userinfo->sex;
			redis_root["icon"] = userinfo->icon;
			RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());//序列化存储

			return true;
		}
	}
	else
	{
		//redis中没有就去mysql中查询
		//查询数据库
		std::shared_ptr<UserInfo> user_info = nullptr;
		user_info = MysqlMgr::GetInstance()->GetUser(uid);
		if (user_info == nullptr)
		{
			return false;
		}

		userinfo = user_info;//更新接收数据对象userinfo
		//将数据内容写入redis缓存
		Json::Value redis_root;
		redis_root["uid"] = uid;
		redis_root["pwd"] = userinfo->pwd;
		redis_root["name"] =userinfo->name;
		redis_root["nick"] = userinfo->nick;
		redis_root["email"] = userinfo->email;
		redis_root["desc"] = userinfo->desc;
		redis_root["sex"] = userinfo->sex;
		redis_root["icon"] = userinfo->icon;
		RedisMgr::GetInstance()->Set(base_key, redis_root.toStyledString());//序列化存储

		
	}
	return true;
}

bool LogicSystem::GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>>& user_list)
{
	//从mysql获取好友列表
	return MysqlMgr::GetInstance()->GetFriendList(self_id,user_list);
}
