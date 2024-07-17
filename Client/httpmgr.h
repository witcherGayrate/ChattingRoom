#ifndef HTTPMGR_H
#define HTTPMGR_H
#include"singleton.h"
#include<QString>
#include<QUrl>
#include<QObject>
#include<QNetworkAccessManager> //网络管理
#include<QJsonObject>
#include<QJsonDocument>//解析Json文件

//CRTP
//HttpMgr是http管理类，直接从单例类模板继承
class HttpMgr:public QObject,public Singleton<HttpMgr>,
                public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT //元对象宏，使用信号和槽
public:
    ~ HttpMgr();//为了让智能智能也能调用析构函数
    //发送http请求
    void PostHttpReq(QUrl url,QJsonObject json,ReqId req_id, Modules mod);
private:
    friend class Singleton<HttpMgr>;//让父类可以访问派生类的构造函数
    HttpMgr();
    QNetworkAccessManager _manager;

private slots:
    void slot_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);//处理sig_http_finish信号的槽函数
signals:
    void sig_http_finish(ReqId id,QString res,ErrorCodes err,Modules mod);
    //注册模块http相关请求发送此信号
    void sig_reg_mod_finish(ReqId id, QString res,ErrorCodes err);
    void sig_reset_mod_finish(ReqId id,QString res,ErrorCodes err);
};



#endif // HTTPMGR_H
