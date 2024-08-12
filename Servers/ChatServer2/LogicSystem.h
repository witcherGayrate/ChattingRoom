#pragma once
#include"const.h"
#include"CSession.h"
#include"data.h"

class CSession;
class LogicNode;
typedef  function<void(shared_ptr<CSession>, const short& msg_id, const string& msg_data)> FunCallBack;//�ص�����ģ��

//�߼�������
class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	//logenode��ӵ�����
	void PostMsgToQue(shared_ptr<LogicNode> msg);
	//�ж��ַ����Ƿ��������
	bool isPureDigit(const std::string& str);
private:
	LogicSystem();
	void DealMsg();
	void RegisterCallBacks();

	//��¼�ص�
	void LoginHandler(shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	//���������ص�
	void SearchInfo(shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	//��mysqlͨ��uid��ѯ�û���Ϣ
	void GetUserByUid(const std::string uid_str, Json::Value& rtValue);
	void GetUserByName(const std::string name, Json::Value& rtValue);
	//��mysql��ѯ�û������б�
	bool GetFriendApplyInfo(const int to_uid, std::vector<std::shared_ptr<ApplyInfo>>& list);
	//��Ӻ�������
	void AddFriendApply(std::shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	//��֤��������
	void AuthFriendApply(std::shared_ptr<CSession>, const short& msg_id, const string& msg_data);
	//��redis��ȡ�û�������Ϣ,û�в鵽�ʹ�mysql�в飬�鵽֮���ٴ浽redis
	bool GetBaseInfo(std::string base_key, int uid, std::shared_ptr<UserInfo>& userinfo);
	//��ȡ�����б�
	bool  GetFriendList(int self_id, std::vector<std::shared_ptr<UserInfo>>& user_list);
	//�����߳�
	std::thread _worker_thread;
	//��Ϣ����
	std::queue<shared_ptr<LogicNode>> _msg_que;
	std::mutex _mutex;
	std::condition_variable _consume;
	//ֹͣ�߳�
	bool _b_stop;
	//�ص�������
	std::map<short, FunCallBack> _fun_callbacks;

};


