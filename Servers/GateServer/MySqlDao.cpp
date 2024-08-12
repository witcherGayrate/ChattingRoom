#include "MySqlDao.h"
#include"ConfigMgr.h"

MySqlPool::MySqlPool(const std::string& url, const std::string& user, const std::string& pass, const std::string& schema, int poolSize)
	:
	user_(user), url_(url), pass_(pass), schema_(schema), poolSize_(poolSize), b_stop_(false)
{
	try
	{
		for (size_t i = 0; i < poolSize_; ++i)
		{
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();//��ȡsql driver
			auto* con = driver->connect(url_, user, pass_);
			con->setSchema(schema_);//���ӵ�schma���ݿ�
			//��ȡ��ǰʱ���
			auto currentTime = std::chrono::system_clock::now().time_since_epoch();
			//��ʱ���ת��Ϊ��
			long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
			pool_.push(std::make_unique<SqlConnection>(con, timestamp));
		}

		_check_thread = std::thread([this]() {
			while (!b_stop_)
			{
				checkConnection();
				std::this_thread::sleep_for(std::chrono::seconds(180));//ÿ3���Ӽ��һ��
			}
			});
		_check_thread.detach();//�߳����������봴�����̶߳����������ڽ��
	}
	catch (const std::exception& e)
	{
		//�����쳣
		std::cout << "mysql pool init failed, error is " << e.what() << std::endl;
	}
}

void MySqlPool::checkConnection()
{
	std::lock_guard<std::mutex> lock(mutex_);
	int poolsize = pool_.size();
	//��ȡ��ǰʱ���
	auto currentTime = std::chrono::system_clock::now().time_since_epoch();
	//��ʱ���ת��Ϊ��
	long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
	for (int i = 0; i < poolsize; i++) {
		auto con = std::move(pool_.front());
		pool_.pop();
		Defer defer([this, &con]() {
			pool_.push(std::move(con));
			});//ÿ��ѭ��������defer������ʱ����ô���Ļص�����

		if (timestamp - con->_last_oper_time < 600) {
			continue;
		}//ʮ����û�в�����

		try {
			std::unique_ptr<sql::Statement> stmt(con->_con->createStatement());
			stmt->executeQuery("SELECT 1");
			con->_last_oper_time = timestamp;
			//std::cout << "execute timer alive query , cur is " << timestamp << std::endl;
		}
		catch (sql::SQLException& e) {
			std::cout << "Error keeping connection alive: " << e.what() << std::endl;
			// ���´������Ӳ��滻�ɵ�����
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
			auto* newcon = driver->connect(url_, user_, pass_);
			newcon->setSchema(schema_);
			con->_con.reset(newcon);//��������sqlConnection�����װ��ԭʼsql::conncetion
			con->_last_oper_time = timestamp;
		}
	}
}

std::unique_ptr<SqlConnection> MySqlPool::getConnection()
{
	std::unique_lock<std::mutex> lock(mutex_);
	//���ӳ�Ϊ�����̹߳���
	cond_.wait(lock, [this]()
		{
			if (b_stop_)
				return true;
			return !pool_.empty();
		});
	if (b_stop_)
	{
		return nullptr;
	}
	std::unique_ptr<SqlConnection> con(std::move(pool_.front()));//�����ӳ���ȡһ��connections
	pool_.pop();
	return con;
}

void MySqlPool::returnConnection(std::unique_ptr<SqlConnection> con)
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (b_stop_)
	{
		return;
	}
	pool_.push(std::move(con));
	cond_.notify_one();
}

void MySqlPool::Close()
{
	b_stop_ = true;
	cond_.notify_all();
}

MySqlPool::~MySqlPool()
{
	std::unique_lock<std::mutex> lock(mutex_);
	while (!pool_.empty())
	{
		pool_.pop();
	}
}

MysqlDao::MysqlDao()
{
	//��ȡ����
	auto& cfg = ConfigMgr::Inst();
	const auto& host = cfg["Mysql"]["Host"];
	const auto& port = cfg["Mysql"]["Port"];
	const auto& pwd = cfg["Mysql"]["Passwd"];
	const auto& schema = cfg["Mysql"]["Schema"];
	const auto& user = cfg["Mysql"]["User"];
	//��ʼ��һ��sql���ӳ�
	pool_.reset(new MySqlPool(host + ":" + port, user, pwd, schema, 5));

}

MysqlDao::~MysqlDao()
{
	pool_->Close();
}

int MysqlDao::RegUser(const std::string& name, const std::string& email, const std::string& pwd)
{
	auto con = pool_->getConnection();
	try
	{
		if (con == nullptr)
		{
			//pool_->returnConnection(std::move(con));
			return false;
		}
		//׼�����ô洢����
		std::unique_ptr<sql::PreparedStatement> stmt(con->_con->prepareStatement("CALL reg_user(?,?,?,@result)"));
		//�����������
		stmt->setString(1, name);
		stmt->setString(2, email);
		stmt->setString(3, pwd);

		//����preparedStatement��ֱ��֧��ע�����������������Ҫʹ�ûỰ����������������ȡ���������ֵ
		stmt->execute();
		// ����洢���������˻Ự��������������ʽ��ȡ���������ֵ�������������ִ��SELECT��ѯ����ȡ����
	   // ���磬����洢����������һ���Ự����@result���洢������������������ȡ��
		std::unique_ptr<sql::Statement> stmtResult(con->_con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @result AS result"));
		if (res->next())
		{
			int result = res->getInt("result");
			std::cout << "Result: " << result << std::endl;
			pool_->returnConnection(std::move(con));//��������
			return result;
		}
		pool_->returnConnection(std::move(con));
		return -1;
	}
	catch (sql::SQLException& e)
	{
		pool_->returnConnection(std::move(con));
		std::cout << "SQL register failed " << std::endl;
		std::cerr << "SQLException: " << e.what();
		std::cerr << "(MySQL error code: " << e.getErrorCode();
		std::cerr << ",SQLState: " << e.getSQLState() << ")" << std::endl;
		return -1;
	}

}

bool MysqlDao::CheckEmail(const std::string& name, const std::string& email)
{
	auto con = pool_->getConnection();
	try
	{
		if (con == nullptr)
		{
			pool_->returnConnection(std::move(con));
			return false;
		}

		// ׼����ѯ���
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT email FROM user WHERE name = ?"));

		//�󶨲���
		pstmt->setString(1, name);
		//ִ�в�ѯ
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		//���������
		while (res->next())
		{
			std::cout << "Check Email: " << res->getString("email") << std::endl;
			if (email != res->getString("email")) {
				pool_->returnConnection(std::move(con));
				return false;
			}
			pool_->returnConnection(std::move(con));
			return true;
		}
	}
	catch (sql::SQLException& e)
	{
		pool_->returnConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
	
}

bool MysqlDao::UpdataPwd(const std::string& name, const std::string& pwd)
{
	auto con = pool_->getConnection();
	try
	{
		if (con == nullptr)
		{
			pool_->returnConnection(std::move(con));
			return false;
		}
		// ׼����ѯ���
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("UPDATE user SET pwd = ? WHERE name = ?"));

		// �󶨲���
		pstmt->setString(2, name);
		pstmt->setString(1, pwd);
		// ִ�и���
		int updateCount = pstmt->executeUpdate();

		std::cout << "Updated rows: " << updateCount << std::endl;
		pool_->returnConnection(std::move(con));
		return true;
	}
	catch (sql::SQLException& e)
	{
		pool_->returnConnection(std::move(con));
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}
}

bool MysqlDao::CheckPwd(const std::string& email, const std::string& pwd, UserInfo& userInfo)
{
	auto con = pool_->getConnection();
	//�ӳٹ黹mysql����
	Defer defer([this, &con]()
		{
			pool_->returnConnection(std::move(con));
		});//defer����ʱ���ø����Ļص�����
	try
	{
		if (con == nullptr) {
			return false;
		}
		//׼��sql���
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT * FROM user WHERE email = ?"));
		pstmt->setString(1, email);//��username�滻Ϊ��Ҫ��ѯ���û���

		//ִ�в�ѯ
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
		std::string origin_pwd = "";
		//���������
		while (res->next())
		{
			origin_pwd = res->getString("pwd");
			//�����ѯ��������
			std::cout << "Password: " << origin_pwd << std::endl;
			break;
		}

		if (pwd != origin_pwd) {
			return false;
		}
		userInfo.name = res->getString("name");
		userInfo.email = email;
		userInfo.uid = res->getInt("uid");
		userInfo.pwd = origin_pwd;
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << "SQLException: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return false;
	}

}
