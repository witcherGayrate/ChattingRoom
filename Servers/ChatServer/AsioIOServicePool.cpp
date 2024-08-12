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
	// TODO: �ڴ˴����� return ���
}

void AsioIOServicePool::Stop()
{
	//workֹͣ���غͷ���
	for (auto& work : _works)
	{
		//��ֹͣ����
		work->get_io_context().stop();
		work.reset();//����ָ��
	}

	//ֹͣ�߳�
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
	//�������io_service����������̣߳�ÿ���߳��ڲ�����io_service
	for (size_t i = 0; i < _ioServices.size(); i++)
	{
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}
}
