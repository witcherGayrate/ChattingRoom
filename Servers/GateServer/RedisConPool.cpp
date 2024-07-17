#include "RedisConPool.h"

RedisConPool::RedisConPool(std::size_t poolSize, const char* host, int port, const char* pwd)
:poolSize_(poolSize), host_(host), port_(port), b_stop_(false)
{
	//初始化redis context
	for (size_t i = 0; i < poolSize; ++i)
	{
		auto* context = redisConnect(host, port);//redisConnect返回的是context
		if (context == nullptr || context->err != 0)
		{
			if (context != nullptr)
			{
				redisFree(context);
			}
			continue;
		}

		auto reply = (redisReply*)redisCommand(context, "AUTH %s", pwd);
		if (reply->type == REDIS_REPLY_ERROR)
		{
			std::cout << "认证失败" << std::endl;
			//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
			freeReplyObject(reply);
			continue;
		}

		//执行成功
		//执行成功 释放redisCommand执行后返回的redisReply所占用的内存
		freeReplyObject(reply);
		std::cout << "认证成功" << std::endl;
		connections_.push(context);
	}
}

//清空连接池
RedisConPool::~RedisConPool()
{
	std::lock_guard<std::mutex> lock(mutex_);
	while (!connections_.empty())
	{
		connections_.pop();
	}
}

/*
	template <class _Predicate>
	void wait(unique_lock<mutex>& _Lck, _Predicate _Pred) { // wait for signal and test predicate
		while (!_Pred()) {
			wait(_Lck);
		}
	}
*/
//用于实现连接池中的资源等待和获取机制。
redisContext* RedisConPool::getConnection()
{
	//生产者-消费者模式
	std::unique_lock<std::mutex> lock(mutex_);//访问队列时必须先获得锁
	cond_.wait(lock, [this]()
		{
			if (b_stop_)
			{
				return true;//线程池关闭
			}
			return !connections_.empty();//队列不为空，继续执行。为空线程继续等待
		});//等待直到被另一个线程通过调用 notify_one 或 notify_all 唤醒，或者直到提供的 lambda 表达式返回 true
	//如果停止则直接返回空指针
	if (b_stop_) {
		return  nullptr;
	}
	auto* context = connections_.front();
	connections_.pop();
	return context;
}//作用域结束，std::unique_lock 对象被销毁，互斥锁在这里释放



void RedisConPool::returnConnection(redisContext* context)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_)
	{
		return;
	}
	connections_.push(context);
	cond_.notify_one(); //归还之后唤醒一个线程
}

void RedisConPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();
}
