#include "LogicSystem.h"
#include"HttpConnection.h"
#include"VerifyGrpcClient.h"
#include"RedisMgr.h"
#include"MysqlMgr.h"
bool LogicSystem::HandleGet(std::string path, std::shared_ptr<HttpConnection> con)
{
	if (_get_handlers.find(path) == _get_handlers.end())
	{
		return false;//û���ҵ����·�ɶ�Ӧ�Ļص�����
	}
	//�ҵ���Ӧ�Ļص�������ֱ�Ӵ���������д���
	_get_handlers[path](con);
	return true;
}

bool LogicSystem::HandlePost(std::string path, std::shared_ptr<HttpConnection> con)
{
	if (_post_handlers.find(path) == _post_handlers.end())
	{
		return false;
	}
	_post_handlers[path](con);
	return true;
}

void LogicSystem::RegGet(std::string url, HttpHandler handler)
{
	_get_handlers.insert(std::make_pair(url, handler));
}

void LogicSystem::RegPost(std::string url, HttpHandler handler)
{
	_post_handlers.insert(std::make_pair(url, handler));
}



LogicSystem::LogicSystem()
{
	RegGet("/get_test",[](std::shared_ptr<HttpConnection> connection) {
		beast::ostream(connection->_response.body()) << "receive get_test req" << std::endl;
		int i = 0;
		for (auto& elem : connection->_get_params)
		{
			i++;
			beast::ostream(connection->_response.body()) << "param" << i << " key is " << elem.first;

			beast::ostream(connection->_response.body()) << ", value is " << i << elem.second << std::endl;
		}

		});//����ʵ����Ϣע��

	RegPost("/get_verifycode", [](std::shared_ptr<HttpConnection> connection)
		{
			auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
			std::cout << "receive body is " << body_str << std::endl;
			//set response
			connection->_response.set(http::field::content_type, "text/json");
			Json::Value root;
			Json::Reader reader;
			Json::Value src_root;
			bool parse_success = reader.parse(body_str, src_root);
			if (!parse_success)
			{
				std::cout << "Failed to parse JSON data! " << std::endl;
				root["error"] = ErrorCodes::Error_Json;
				std::string jsonstr = root.toStyledString();
				beast::ostream(connection->_response.body()) << jsonstr;
				return true;
			}
			auto email = src_root["email"].asString();
			//grpcclient��Զ�̵���nodejs��̨��Զ��service GetVerifyCode
			GetVerifyRsp rsp = VerifyGrpcClient::GetInstance()->GetVerifyCode(email);
			std::cout << "email is " << email << std::endl;
			std::cout << "error of rsp: " << rsp.error()<<std::endl;
			root["error"] = rsp.error();
			root["email"] = src_root["email"];
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}
	);

	//ע��"/user_register"��Ӧ��post����
	RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection)
		{
			auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
			std::cout << "receive body is " << body_str << std::endl;
			connection->_response.set(http::field::content_type, "text/json");
			Json::Value root;
			Json::Reader reader;
			Json::Value src_root;
			bool parse_success = reader.parse(body_str, src_root);
			if (!parse_success)
			{
				std::cout << "Failed to parse JSON data!" << std::endl;
				root["error"] = ErrorCodes::Error_Json;
				std::string jsonstr = root.toStyledString();
				beast::ostream(connection->_response.body()) << jsonstr;
				return true;
			}

			//
			auto email = src_root["email"].asString();
			auto name = src_root["user"].asString();
			auto pwd = src_root["passwd"].asString();
			auto confirm = src_root["confirm"].asString();

			if(pwd!=confirm) {
				std::cout << "password err " << std::endl;
				root["error"] = ErrorCodes::PasswdErr;
				std::string jsonstr = root.toStyledString();
				beast::ostream(connection->_response.body()) << jsonstr;
				return true;
			}

			//�Ȳ���redis�е�email��Ӧ����֤���Ƿ����
			std::string  verify_code;
			bool b_get_verify = RedisMgr::GetInstance()->Get(CODEPREFIX+src_root["email"].asString(), verify_code);//(email,verify_code)
			if (!b_get_verify)
			{
				std::cout << "get verify code expired " << std::endl;
				root["error"] = ErrorCodes::VerifyExpired;
				std::string jsonstr = root.toStyledString();
				beast::ostream(connection->_response.body()) << jsonstr;
				return true;
			}

			if (verify_code != src_root["verifycode"].asString())
			{
				std::cout << " verify code error " << std::endl;
				root["error"] = ErrorCodes::VerifyCodeErr;
				std::string jsonstr = root.toStyledString();
				beast::ostream(connection->_response.body()) << jsonstr;
				return true;
			}

			
			//�������ݿ��ж��û��Ƿ����
			int uid = MysqlMgr::GetInstance()->RegUser(name, email, pwd);
			if (uid == 0 || uid == -1)
			{
				//std::cout << " user or email exist" << std::endl;
				root["error"] = ErrorCodes::UserExist;
				std::string jsonstr = root.toStyledString();
				beast::ostream(connection->_response.body()) << jsonstr;
				return true;
			}
			root["error"] = 0;
			root["uid"] = uid;
			root["email"] = email;
			root["user"] = name;
			root["passwd"] = pwd;
			root["confirm"] = confirm;
			root["verifycode"] = src_root["verifycode"].asString();
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		});
	//ע��"/reset_pwd" ��post����
	RegPost("/reset_pwd", [](std::shared_ptr<HttpConnection> connection) {
		auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
		std::cout << " receive body is " << body_str << std::endl;
		connection->_response.set(http::field::content_type, "text/json");//������������
		Json::Value root;
		Json::Reader reader;
		Json::Value src_root;
		bool parse_success = reader.parse(body_str, src_root);
		if (!parse_success) {
			std::cout << "Failed to parse JSON data!" << std::endl;
			root["error"] = ErrorCodes::Error_Json;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		auto email = src_root["email"].asString();
		auto name = src_root["user"].asString();
		auto pwd = src_root["passwd"].asString();
		//�Ȳ���redis��email��Ӧ�� ��֤���Ƿ����
		std::string verify_code;
		bool b_get_verify = RedisMgr::GetInstance()->Get(CODEPREFIX + src_root["email"].asString(),
			verify_code);
		if (!b_get_verify)
		{
			std::cout << "get verify code expired" << std::endl;
			root["error"] = ErrorCodes::VerifyExpired;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		if (verify_code != src_root["verifycode"].asString()) {
			std::cout << " verify code error" << std::endl;
			root["error"] = ErrorCodes::VerifyCodeErr;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		//��ѯ���ݿ��ж��û����������Ƿ�ƥ��
		bool email_valid = MysqlMgr::GetInstance()->CheckEmail(name, email);
		if (!email_valid)
		{
			std::cout << " user email not match" << std::endl;
			root["error"] = ErrorCodes::EmailNotMatch;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		//��������Ϊ��������
		bool b_up = MysqlMgr::GetInstance()->UpdatePwd(name, pwd);
		if (!b_up)
		{
			std::cout << " update pwd failed" << std::endl;
			root["error"] = ErrorCodes::PasswdUpFailed;
			std::string jsonstr = root.toStyledString();
			beast::ostream(connection->_response.body()) << jsonstr;
			return true;
		}

		std::cout << "succeed to update password" << pwd << std::endl;
		root["error"] = 0;
		root["email"] = email;
		root["user"] = name;
		root["passwd"] = pwd;
		root["verifycode"] = src_root["verifycode"].asString();
		std::string jsonstr = root.toStyledString();
		beast::ostream(connection->_response.body()) << jsonstr;
		return true;

		});
}
