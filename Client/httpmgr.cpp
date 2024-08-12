#include "httpmgr.h"

HttpMgr::~HttpMgr()
{

}
HttpMgr::HttpMgr()
{
    //连接信号槽，request请求发送并接收回应之后，发射带参信号（由registerdialog捕获并处理）
    connect(this,&HttpMgr::sig_http_finish,this,&HttpMgr::slot_http_finish);

}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    //创建一个HTTP POST请求，并设置请求头和请求体
    QByteArray data = QJsonDocument(json).toJson();//把json对象转为字节流
    QNetworkRequest request(url);//QNetworkRequest类提供http request，设置请求的url
    //http请求头格式RHAUCC
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");//设置请求头内容的类型
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length()));//设置请求头的内容长度
    auto self = shared_from_this();//防止回调时，this对象已被销毁
    QNetworkReply *reply = _manager.post(request,data);//network manager 发送request并接收response
    //建立response完成的回调函数连接，回调函数发送信号传出respnse的req_id,res,errorcode,module
    QObject::connect(reply,&QNetworkReply::finished,[self,reply,req_id,mod](){
        //处理错误情况
        if(reply->error()!=QNetworkReply::NoError)
        {
            qDebug()<<reply->errorString();
            //发送信号通知完成
            emit self->sig_http_finish(req_id,"",ErrorCodes::ERR_NETWORK,mod);
            reply->deleteLater(); //稍后删除reply
            return;
        }
        //无错误
        QString res = reply->readAll();
        //发送信号通知完成
        emit self->sig_http_finish(req_id,res,ErrorCodes::SUCCESS,mod);
        reply->deleteLater();
        return;

    });
}

//singal sig_http_finish的回调函数,参数与之保持一致
void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if(mod == Modules::REGISTERMOD)
    {
        //发送信号通知指定模块http的响应结束了
        emit sig_reg_mod_finish(id, res,err);
    }
    //重置密码模块http响应结束
    if(mod == Modules::RESETMOD)
    {
        emit sig_reset_mod_finish(id,res,err);
    }
    //登录模块的http响应
    if(mod == Modules::LOGINMOD)
    {
        emit sig_login_mod_finish(id,res,err);
    }

}
