#pragma once
#include"const.h"

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
	//使用round-robin方式返回一个io_service
	IOService& GetIOService();
	void Stop();
private:
	AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());

	std::vector<IOService> _ioServices;
	std::vector<WorkPtr> _works;//保证io在没有读写事件也能运行
	std::vector<std::thread> _threads;
	std::size_t _nextIOService;
};

