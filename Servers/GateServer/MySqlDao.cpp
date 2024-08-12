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
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();//获取sql driver
			auto* con = driver->connect(url_, user, pass_);
			con->setSchema(schema_);//连接到schma数据库
			//获取当前时间戳
			auto currentTime = std::chrono::system_clock::now().time_since_epoch();
			//将时间戳转换为秒
			long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
			pool_.push(std::make_unique<SqlConnection>(con, timestamp));
		}

		_check_thread = std::thread([this]() {
			while (!b_stop_)
			{
				checkConnection();
				std::this_thread::sleep_for(std::chrono::seconds(180));//每3分钟检查一次
			}
			});
		_check_thread.detach();//线程生命周期与创建的线程对象生命周期解绑
	}
	catch (const std::exception& e)
	{
		//处理异常
		std::cout << "mysql pool init failed, error is " << e.what() << std::endl;
	}
}

void MySqlPool::checkConnection()
{
	std::lock_guard<std::mutex> lock(mutex_);
	int poolsize = pool_.size();
	//获取当前时间戳
	auto currentTime = std::chrono::system_clock::now().time_since_epoch();
	//将时间戳转换为秒
	long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(currentTime).count();
	for (int i = 0; i < poolsize; i++) {
		auto con = std::move(pool_.front());
		pool_.pop();
		Defer defer([this, &con]() {
			pool_.push(std::move(con));
			});//每次循环结束，defer在析构时会调用传入的回调函数

		if (timestamp - con->_last_oper_time < 600) {
			continue;
		}//十分钟没有操作了

		try {
			std::unique_ptr<sql::Statement> stmt(con->_con->createStatement());
			stmt->executeQuery("SELECT 1");
			con->_last_oper_time = timestamp;
			//std::cout << "execute timer alive query , cur is " << timestamp << std::endl;
		}
		catch (sql::SQLException& e) {
			std::cout << "Error keeping connection alive: " << e.what() << std::endl;
			// 重新创建连接并替换旧的连接
			sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
			auto* newcon = driver->connect(url_, user_, pass_);
			newcon->setSchema(schema_);
			con->_con.reset(newcon);//重新设置sqlConnection里面封装的原始sql::conncetion
			con->_last_oper_time = timestamp;
		}
	}
}

std::unique_ptr<SqlConnection> MySqlPool::getConnection()
{
	std::unique_lock<std::mutex> lock(mutex_);
	//连接池为空则线程挂起
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
	std::unique_ptr<SqlConnection> con(std::move(pool_.front()));//从连接池拿取一个connections
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
	//读取配置
	auto& cfg = ConfigMgr::Inst();
	const auto& host = cfg["Mysql"]["Host"];
	const auto& port = cfg["Mysql"]["Port"];
	const auto& pwd = cfg["Mysql"]["Passwd"];
	const auto& schema = cfg["Mysql"]["Schema"];
	const auto& user = cfg["Mysql"]["User"];
	//初始化一个sql连接池
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
		//准备调用存储过程
		std::unique_ptr<sql::PreparedStatement> stmt(con->_con->prepareStatement("CALL reg_user(?,?,?,@result)"));
		//设置输入参数
		stmt->setString(1, name);
		stmt->setString(2, email);
		stmt->setString(3, pwd);

		//由于preparedStatement不直接支持注册输出参数，我们需要使用会话变量或其他方法获取输出参数的值
		stmt->execute();
		// 如果存储过程设置了会话变量或有其他方式获取输出参数的值，你可以在这里执行SELECT查询来获取它们
	   // 例如，如果存储过程设置了一个会话变量@result来存储输出结果，可以这样获取：
		std::unique_ptr<sql::Statement> stmtResult(con->_con->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmtResult->executeQuery("SELECT @result AS result"));
		if (res->next())
		{
			int result = res->getInt("result");
			std::cout << "Result: " << result << std::endl;
			pool_->returnConnection(std::move(con));//返回连接
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

		// 准备查询语句
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT email FROM user WHERE name = ?"));

		//绑定参数
		pstmt->setString(1, name);
		//执行查询
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

		//遍历结果集
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
		// 准备查询语句
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("UPDATE user SET pwd = ? WHERE name = ?"));

		// 绑定参数
		pstmt->setString(2, name);
		pstmt->setString(1, pwd);
		// 执行更新
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
	//延迟归还mysql连接
	Defer defer([this, &con]()
		{
			pool_->returnConnection(std::move(con));
		});//defer析构时调用给定的回调函数
	try
	{
		if (con == nullptr) {
			return false;
		}
		//准备sql语句
		std::unique_ptr<sql::PreparedStatement> pstmt(con->_con->prepareStatement("SELECT * FROM user WHERE email = ?"));
		pstmt->setString(1, email);//将username替换为你要查询的用户名

		//执行查询
		std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
		std::string origin_pwd = "";
		//遍历结果集
		while (res->next())
		{
			origin_pwd = res->getString("pwd");
			//输出查询到的密码
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
