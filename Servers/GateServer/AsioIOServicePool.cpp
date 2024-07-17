#include "AsioIOServicePool.h"
#include<iostream>
AsioIOServicePool::~AsioIOServicePool()
{
	Stop();
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
	//仅执行work.reset并不能让io_context从run的状态中退出
	//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务
	for (auto& work : _works)
	{
		//服务先停
		work->get_io_context().stop();
		work.reset();
	}
	//等待线程结束并清理资源
	for (auto& t : _threads)
	{
		t.join();
	}
}

AsioIOServicePool::AsioIOServicePool(std::size_t size):_ioServices(size),
_works(size),_nextIOService(0)
{
	//work bind io_context
	for (std::size_t i = 0; i < size; ++i)
	{
		_works[i] = std::unique_ptr<Work>(new Work(_ioServices[i]));
	}

	//把每个io_context分别交给一个thread处理
	for (std::size_t i = 0; i < _ioServices.size(); i++)
	{
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}
