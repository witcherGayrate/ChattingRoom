#include "AsioIOServicePool.h"

AsioIOServicePool::~AsioIOServicePool()
{
	std::cout << "AsioIOServicePool destruct" << std::endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService()
{
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size())
	{
		_nextIOService = 0;
	}
	return service;
	// TODO: 在此处插入 return 语句
}

void AsioIOServicePool::Stop()
{
	//work停止看守和服务
	for (auto& work : _works)
	{
		//先停止服务
		work->get_io_context().stop();
		work.reset();//重置指针
	}

	//停止线程
	for (auto& thread : _threads)
	{
		thread.join();
	}
}

AsioIOServicePool::AsioIOServicePool(std::size_t size):_nextIOService(0),_works(size),_ioServices(size)
{
	for (size_t i = 0; i < size; i++)
	{
		_works[i] = std::unique_ptr<Work>(new Work(_ioServices[i]));
	}
	//遍历多个io_service，创建多个线程，每个线程内部启动io_service
	for (size_t i = 0; i < _ioServices.size(); i++)
	{
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}
