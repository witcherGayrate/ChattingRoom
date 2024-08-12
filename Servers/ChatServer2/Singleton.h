#pragma once
#include<memory>
#include<mutex>
#include<iostream>

template<typename T>
class Singleton
{
protected:
	Singleton() = default;
	Singleton(const Singleton<T>&) = delete;
	Singleton<T>& operator=(const Singleton<T>& st) = delete;

	static std::shared_ptr<T> _instance;//静态T类型实例
public:
	static std::shared_ptr<T> GetInstance()
	{
		static std::once_flag s_flag;//静态局部变量s_flag
		std::call_once(s_flag, [&]()
			{
				_instance = std::shared_ptr<T>(new T);
			});

		return _instance;
	}

	void PrintAddress()
	{
		std::cout << _instance.get() << std::endl;
	}
	~Singleton()
	{
		std::cout << "this is singleton destruct " << std::endl;
	}

};
//初始化静态变量
template<typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;
