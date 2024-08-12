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
	//再次登录可能是其他服务器，所以会造成本服务器删除key，其他服务器注册key的情况
	// 可能其他服务登录，本地删除key造成找不到key的情况
	RedisMgr::GetInstance()->Del(USERIPPREFIX + uid_str);
	//上锁
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_uid_to_session.erase(uid);
	}
}

UserMgr::UserMgr()
{
}
