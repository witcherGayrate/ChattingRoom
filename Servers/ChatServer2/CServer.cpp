#include "CServer.h"
#include"UserMgr.h"
CServer::CServer(boost::asio::io_context& io_context, short port):_io_context(io_context),_port(port),
_accpetor(io_context,tcp::endpoint(tcp::v4(),port))
{
	std::cout << "Server start success,listen on port: " << _port << std::endl;
	StartAccept();
}

CServer::~CServer()
{
	std::cout<< "Server destruct listen on port : " << _port << std::endl;
}

void CServer::ClearSession(std::string session_id)
{
	if (_sessions.find(session_id) != _sessions.end()) {
		//移除用户和session的关联
		UserMgr::GetInstance()->RmvUserSession(_sessions[session_id]->GetUserId());

	}
	
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_sessions.erase(session_id);
	}

}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code& error)
{
	if (!error)
	{
		new_session->Start();
		std::lock_guard<std::mutex> lock(_mutex);
		_sessions.insert(std::make_pair(new_session->GetSessionId(),new_session ));

	}
	else {
		std::cout << "session accept failed, error is " << error.what() << std::endl;
	}
	//继续接收
	StartAccept();
}

void CServer::StartAccept()
{
	//从iocontext pool中获取一个io_context用以client通信
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);
	_accpetor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session,
		std::placeholders::_1));
}
