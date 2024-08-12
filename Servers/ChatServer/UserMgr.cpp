#include "UserMgr.h"
#include"RedisMgr.h"
#include"CSession.h"
UserMgr::~UserMgr()
{
	std::lock_guard<std::mutex> lock(_mutex);
	_uid_to_session.clear();
}

std::shared_ptr<CSession> UserMgr::GetSession(int uid)
{
	std::lock_guard<std::mutex> lock(_mutex);
	auto iter = _uid_to_session.find(uid);
	if (iter != _uid_to_session.end())
	{
		return iter->second;
	}
	else {
		return nullptr;
	}
}

void UserMgr::SetUserSession(int uid, std::shared_ptr<CSession> session)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_uid_to_session[uid] = session;
}

void UserMgr::RmvUserSession(int uid)
{
	auto uid_str = std::to_string(uid);
	//�ٴε�¼���������������������Ի���ɱ�������ɾ��key������������ע��key�����
	// �������������¼������ɾ��key����Ҳ���key�����
	RedisMgr::GetInstance()->Del(USERIPPREFIX + uid_str);
	//����
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_uid_to_session.erase(uid);
	}
}

UserMgr::UserMgr()
{
}
