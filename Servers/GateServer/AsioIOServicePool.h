#pragma once
#include"Singleton.h"
#include<vector>
#include<boost/asio.hpp>

class AsioIOServicePool:public Singleton<AsioIOServicePool>
{
	friend class Singleton<AsioIOServicePool>;
public:
	using IOService = boost::asio::io_context;
	using Work = boost::asio::io_context::work;
	using WorkPtr = std::unique_ptr<Work>;
	~AsioIOServicePool();
	AsioIOServicePool(const AsioIOServicePool&) = delete;
	AsioIOServicePool& operator=(const AsioIOServicePool&) = delete;
	//��ѯ��ȡio_service
	boost::asio::io_context& GetIOService();
	void Stop();
private:
	AsioIOServicePool(std::size_t size = 4/*std::thread::hardware_concurrency()*/);

	std::vector<IOService> _ioServices; //io������
	std::vector<WorkPtr> _works;//ʹ��io�������е�workָ��
	std::vector<std::thread> _threads;//������io���̳߳�
	std::size_t _nextIOService;
};

