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
	//轮询获取io_service
	boost::asio::io_context& GetIOService();
	void Stop();
private:
	AsioIOServicePool(std::size_t size = 4/*std::thread::hardware_concurrency()*/);

	std::vector<IOService> _ioServices; //io上下文
	std::vector<WorkPtr> _works;//使得io保持运行的work指针
	std::vector<std::thread> _threads;//管理单个io的线程池
	std::size_t _nextIOService;
};

