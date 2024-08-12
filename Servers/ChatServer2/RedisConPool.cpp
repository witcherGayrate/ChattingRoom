#include "RedisConPool.h"

RedisConPool::RedisConPool(std::size_t poolSize, const char* host, int port, const char* pwd)
:poolSize_(poolSize), host_(host), port_(port), b_stop_(false)
{
	//��ʼ��redis context
	for (size_t i = 0; i < poolSize; ++i)
	{
		auto* context = redisConnect(host, port);//redisConnect���ص���context
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
			std::cout << "��֤ʧ��" << std::endl;
			//ִ�гɹ� �ͷ�redisCommandִ�к󷵻ص�redisReply��ռ�õ��ڴ�
			freeReplyObject(reply);
			continue;
		}

		//ִ�гɹ�
		//ִ�гɹ� �ͷ�redisCommandִ�к󷵻ص�redisReply��ռ�õ��ڴ�
		freeReplyObject(reply);
		std::cout << "��֤�ɹ�" << std::endl;
		connections_.push(context);
	}
}

//������ӳ�
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
//����ʵ�����ӳ��е���Դ�ȴ��ͻ�ȡ���ơ�
redisContext* RedisConPool::getConnection()
{
	//������-������ģʽ
	std::unique_lock<std::mutex> lock(mutex_);//���ʶ���ʱ�����Ȼ����
	cond_.wait(lock, [this]()
		{
			if (b_stop_)
			{
				return true;//�̳߳عر�
			}
			return !connections_.empty();//���в�Ϊ�գ�����ִ�С�Ϊ���̼߳����ȴ�
		});//�ȴ�ֱ������һ���߳�ͨ������ notify_one �� notify_all ���ѣ�����ֱ���ṩ�� lambda ���ʽ���� true
	//���ֹͣ��ֱ�ӷ��ؿ�ָ��
	if (b_stop_) {
		return  nullptr;
	}
	auto* context = connections_.front();
	connections_.pop();
	return context;
}//�����������std::unique_lock �������٣��������������ͷ�



void RedisConPool::returnConnection(redisContext* context)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_)
	{
		return;
	}
	connections_.push(context);
	cond_.notify_one(); //�黹֮����һ���߳�
}

void RedisConPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();
}
