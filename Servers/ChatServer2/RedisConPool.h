#pragma once
#include"const.h"
class RedisConPool
{
public:
	//建立rediscontext的连接池
	RedisConPool(std::size_t poolSize, const char* host, int port, const char* pwd);
	~RedisConPool();

	//从连接池队列获取redisContext
	redisContext* getConnection();
	//用完之后还给redisPool
	void returnConnection(redisContext* context);

	void Close();
private:
	std::mutex mutex_;//互斥量
	std::condition_variable cond_;//条件变量
	std::atomic<bool> b_stop_;//原子变量

	std::size_t poolSize_;//连接池大小
	const char* host_;//redis服务器ip地址
	int port_;//端口号
	std::queue<redisContext*> connections_;//rediscontext队列


};

