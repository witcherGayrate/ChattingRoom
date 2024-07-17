#include "CServer.h"
#include"AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc,tcp::endpoint(tcp::v4(),port)),_socket(ioc) //接收器用ioc上下文和endpoint初始化
{
	std::cout << "GateServer start to listen on port:8080"<<std::endl;
}

void CServer::Start()
{
	auto self = shared_from_this();
	//现在改为从线程池获取io上下文
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	//接收器异步接收连接，有新连接时初始化_socket，并将与peer通信的_socket move给httpconnection类，达到更新_socket的目的
	_acceptor.async_accept(new_con->GetSocket(), [self,new_con](beast::error_code ec)//_socket引用
		{
			try
			{
				if (ec)
				{
					//如果出错放弃这个连接，继续监听新链接
					self->Start();
					return;
				}
				//处理新连接，创建HpptConnection类管理新连接
				//std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
				new_con->Start();
				//继续监听
				self->Start();
			}
			catch (const std::exception& exp)
			{
				std::cout << "exception is " << exp.what() << std::endl;
				//继续监听
				self->Start();
			}
		});
}