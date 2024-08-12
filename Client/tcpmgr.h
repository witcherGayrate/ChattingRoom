#ifndef TCPMGR_H
#define TCPMGR_H
#include"singleton.h"
#include<QTcpSocket>
#include <QObject>
#include"global.h"
#include<functional>
#include<QJsonDocument>
#include<QJsonObject>
#include"userdata.h"


class TcpMgr : public QObject,public Singleton<TcpMgr>,
               public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
    TcpMgr();
private:
    friend class Singleton<TcpMgr>;
    QTcpSocket _socket;
    QString _host;
    uint16_t _port;
    QByteArray _buffer;
    bool _b_recv_pending;//是否停止解析
    quint16 _message_id;
    quint16 _message_len;

    void initHandlers();
    void handleMsg(ReqId id,int len,QByteArray data);//根据收到的msg reqid决定调用哪个handler
    QMap<ReqId,std::function<void(ReqId,int,QByteArray)>> _handlers;//回调处理map表
public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QByteArray data);//QString改为QByteArray


signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqId, QByteArray data);
    void sig_login_failed(int);
    void sig_switch_chatdlg();
    void sig_user_search(std::shared_ptr<SearchInfo>);
    void sig_auth_rsp(std::shared_ptr<AuthRsp>);//同意对方申请好友的信号
    void sig_add_auth_friend(std::shared_ptr<AuthInfo>);//对方同意为好友
    void sig_friend_apply(std::shared_ptr<AddFriendApply>);//对方申请好友的信号


};

#endif // TCPMGR_H
