#pragma once
#include"const.h"
class RedisConPool
{
public:
	//����rediscontext�����ӳ�
	RedisConPool(std::size_t poolSize, const char* host, int port, const char* pwd);
	~RedisConPool();

	//�����ӳض��л�ȡredisContext
	redisContext* getConnection();
	//����֮�󻹸�redisPool
	void returnConnection(redisContext* context);

	void Close();
private:
	std::mutex mutex_;//������
	std::condition_variable cond_;//��������
	std::atomic<bool> b_stop_;//ԭ�ӱ���

	std::size_t poolSize_;//���ӳش�С
	const char* host_;//redis������ip��ַ
	int port_;//�˿ں�
	std::queue<redisContext*> connections_;//rediscontext����


};

