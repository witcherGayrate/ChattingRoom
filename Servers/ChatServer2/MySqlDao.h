#pragma once
#include"const.h"
#include"data.h"
//SqlConnection�з�װ��sql::Connection��
class SqlConnection
{
public:
	SqlConnection(sql::Connection* con, int64_t lasttime) :_con(con), _last_oper_time(lasttime) {}
	std::unique_ptr<sql::Connection> _con;
	int64_t _last_oper_time;//�ϴβ�����ʱ��
};

class MySqlPool
{
public:
	MySqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolSize);
	void checkConnection();
	std::unique_ptr<SqlConnection> getConnection();
	void returnConnection(std::unique_ptr<SqlConnection> con);
	void Close();
	~MySqlPool();
private:
	std::string url_;
	std::string user_;
	std::string pass_;
	std::string schema_; //mysql�е����ݿ���wechat
	int  poolSize_;
	std::queue<std::unique_ptr<SqlConnection>> pool_;
	std::mutex mutex_;
	std::condition_variable cond_;
	std::atomic<bool> b_stop_;
	std::thread _check_thread; //��ⳬʱ�߳�
};


class MysqlDao
{
public:
	MysqlDao();
	~MysqlDao();
	int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
	bool CheckEmail(const std::string& name, const std::string& email);
	bool UpdataPwd(const std::string& name, const std::string& pwd);
	bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userinfo);
	bool AddFriendApply(const int from, const int touid);
	std::shared_ptr<UserInfo> GetUser(int uid);
	std::shared_ptr<UserInfo> GetUser(std::string name);
	bool GetApplyList(int touid, std::vector<std::shared_ptr<ApplyInfo>>& applyList, int begin, int limit = 10);
	bool AuthFriendApply(const int& from, const int& to);
	bool AddFriend(const int& from, const int& to, std::string bak_name);
	bool GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>>& user_list);//��ȡ�����б�
private:
	std::unique_ptr<MySqlPool> pool_;
};