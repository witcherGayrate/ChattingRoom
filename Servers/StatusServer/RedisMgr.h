#pragma once
#include"const.h"
#include"RedisConPool.h"
class RedisMgr :public Singleton<RedisMgr>,
	std::enable_shared_from_this<RedisMgr>
{
	friend class Singleton<RedisMgr>;
public:
	~RedisMgr();
	
	bool Get(const std::string& key, std::string& value);
	bool Set(const std::string& key, const std::string& value);
	//bool Auth(const std::string& password);
	//LPUSH设置列表key的序列值
	bool LPush(const std::string& key, const std::string& value);
	bool LPop(const std::string& key, std::string& value);
	bool RPush(const std::string& key, const std::string& value);
	bool RPop(const std::string& key, std::string& value);
	//HSet设置散列表hashes的key-value
	bool HSet(const std::string& key, const std::string& hkey, const std::string& value);
	bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen);

	std::string HGet(const std::string& key, const std::string& hkey);
	bool Del(const std::string& key);
	bool HDel(const std::string& key, const std::string& field);
	bool ExistsKey(const std::string& key);
	void Close();
private:
	RedisMgr();
	//连接池redisPool
	std::unique_ptr<RedisConPool> _con_pool;
};

