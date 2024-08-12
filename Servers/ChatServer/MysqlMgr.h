#pragma once
#include"const.h"
#include"MySqlDao.h"
class MysqlMgr :public Singleton<MysqlMgr>
{
	friend class Singleton<MysqlMgr>;
public:
	~MysqlMgr();
	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
	bool CheckEmail(const std::string& name, const std::string& email);
	bool UpdatePwd(const std::string name, const std::string& pwd);
	bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userinfo);
	//添加好友申请的记录
	bool AddFriendApply(const int from, const int touid);
	std::shared_ptr<UserInfo> GetUser(int uid);
	std::shared_ptr<UserInfo> GetUser(std::string name);
	bool GetApplyList(int touid, std::vector<std::shared_ptr<ApplyInfo>>& applyList, int begin, int limit = 10);
	bool AuthFriendApply(const int& from, const int& to);
	bool AddFriend(const int& from, const int& to, std::string bak_name);
	bool GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>>& user_list);
private:
	MysqlMgr();
	MysqlDao _dao;
};

