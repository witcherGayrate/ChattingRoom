#include "CServer.h"
#include"AsioIOServicePool.h"
CServer::CServer(boost::asio::io_context& ioc, unsigned short& port):_ioc(ioc),
_acceptor(ioc,tcp::endpoint(tcp::v4(),port)),_socket(ioc) //��������ioc�����ĺ�endpoint��ʼ��
{
	std::cout << "GateServer start to listen on port:8080"<<std::endl;
}

void CServer::Start()
{
	auto self = shared_from_this();
	//���ڸ�Ϊ���̳߳ػ�ȡio������
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);
	//�������첽�������ӣ���������ʱ��ʼ��_socket��������peerͨ�ŵ�_socket move��httpconnection�࣬�ﵽ����_socket��Ŀ��
	_acceptor.async_accept(new_con->GetSocket(), [self,new_con](beast::error_code ec)//_socket����
		{
			try
			{
				if (ec)
				{
					//����������������ӣ���������������
					self->Start();
					return;
				}
				//���������ӣ�����HpptConnection�����������
				//std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
				new_con->Start();
				//��������
				self->Start();
			}
			catch (const std::exception& exp)
			{
				std::cout << "exception is " << exp.what() << std::endl;
				//��������
				self->Start();
			}
		});
}