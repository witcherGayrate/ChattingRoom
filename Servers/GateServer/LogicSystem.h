#pragma once
#include"Singleton.h"
#include<functional>
#include<map>
#include"const.h"

class HttpConnection;
typedef std::function<void(std::shared_ptr<HttpConnection>)> HttpHandler;//�洢httpconnection�Ļص���������

class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem() {};//����ָ����Ե���
	bool HandleGet(std::string, std::shared_ptr<HttpConnection>);//����get�������͵Ļص�
	bool HandlePost(std::string path, std::shared_ptr<HttpConnection> con);
	void RegGet(std::string, HttpHandler handler);//ʵ����Ϣע�� ��RegGet����������·�ɺͻص�������Ϊ����
	void RegPost(std::string, HttpHandler handler);//ʵ��post��handle
private:
	LogicSystem();
	std::map < std::string, HttpHandler > _post_handlers; //post�����Ӧ�ĺ�����,keyΪ·�ɣ�valueΪ�ص�����
	std::map<std::string, HttpHandler> _get_handlers;//get�����Ӧ�ĺ�����,keyΪ·�ɣ�valueΪ�ص�����
};

