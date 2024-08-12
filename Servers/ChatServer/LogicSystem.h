#pragma once
#include"const.h"
#include"CSession.h"
#include"data.h"

class CSession;
class LogicNode;
typedef  function<void(shared_ptr<CSession>, const short& msg_id, const string& msg_data)> FunCallBack;//回调函数模板

//逻辑处理单例
class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	//logenode添加到队列
	void PostMsgToQue(shared_ptr<LogicNode> msg);
	//判断字符串是否包含数字
	bool isPureDigit(const std::string& str);
private:
	LogicSystem();
	void DealMsg();
	void RegisterCallBacks();

	//登录回调
	void LoginHandler(shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	//好友搜索回调
	void SearchInfo(shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	//从mysql通过uid查询用户信息
	void GetUserByUid(const std::string uid_str, Json::Value& rtValue);
	void GetUserByName(const std::string name, Json::Value& rtValue);
	//从mysql查询用户申请列表
	bool GetFriendApplyInfo(const int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& list);
	//添加好友请求
	void AddFriendApply(std::shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	//认证好友请求
	void AuthFriendApply(std::shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	//从redis获取用户基本信息,没有查到就从mysql中查，查到之后再存到redis
	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
	//获取好友列表
	bool  GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>>& user_list);
	//消费线程
	std::thread _worker_thread;
	//消息队列
	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::condition_variable _consume;
	//停止线程
	bool _b_stop;
	//回调函数表
	std::map<short, FunCallBack> _fun_callbacks;

};


