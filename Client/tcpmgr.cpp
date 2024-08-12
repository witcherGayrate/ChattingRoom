#include "tcpmgr.h"
#include<QJsonArray>
#include"usermgr.h"

TcpMgr::TcpMgr():_host(""),_port(0),_b_recv_pending(false),_message_id(0),_message_len(0)
{
    QObject::connect(&_socket, &QTcpSocket::connected, [&]() {
        qDebug() << "Connected to server!";
        // 连接建立后发送消息
        emit sig_con_success(true);
    });

    QObject::connect(&_socket, &QTcpSocket::readyRead, [&]() {
        // 当有数据可读时，读取所有数据
        // 读取所有数据并追加到缓冲区
        _buffer.append(_socket.readAll());//QBytesArray
        QDataStream stream(&_buffer, QIODevice::ReadOnly);//字节流转为二进制
        stream.setVersion(QDataStream::Qt_6_0);
        forever {//无限循环
            //先解析头部
            if(!_b_recv_pending){
                // 检查缓冲区中的数据是否足够解析出一个消息头（消息ID + 消息长度）
                if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2)) {
                    return; // 数据不够，等待更多数据
                }
                // 预读取消息ID和消息长度，但不从缓冲区中移除
                stream >> _message_id >> _message_len;
                //将buffer 中的前四个字节移除
                _buffer = _buffer.mid(sizeof(quint16) * 2);
                // 输出读取的数据
                qDebug() << "Message ID:" << _message_id << ", Length:" << _message_len;
            }
            //buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
            if(_buffer.size() < _message_len){
                _b_recv_pending = true;
                return;
            }
            _b_recv_pending = false;//能够完整读取一个消息体，则停止接收消息
            // 读取消息体
            QByteArray messageBody = _buffer.mid(0, _message_len);
            qDebug() << "receive body msg is " << messageBody ;

            _buffer = _buffer.mid(_message_len);//读后截取掉
            handleMsg(ReqId(_message_id),_message_len,messageBody);
        }
    });

    //5.15 之后版本
    QObject::connect(&_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
        qDebug() << "Error:" << _socket.errorString();
    });

    // 处理连接断开
    QObject::connect(&_socket, &QTcpSocket::disconnected, [&]() {
        qDebug() << "Disconnected from server.";
    });

    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
    //注册消息
    initHandlers();
}

void TcpMgr::initHandlers()
{
    //处理登录请求回包
    _handlers.insert(ID_CHAT_LOGIN_RSP,[this](ReqId id,int len,QByteArray data){

        Q_UNUSED(len);
        qDebug()<<"handle id is "<<id<<"data is "<<data;
        //将QByteArray转为QJSONDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        //检查转换是否成功
        if(jsonDoc.isNull())
        {
            qDebug()<<"failed to create QJsonDocument";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error"))
            {
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Login Failed,err is Json Parse Err"<<err;
            emit sig_login_failed(err);
            return;
        }
        int err = jsonObj["error"].toInt();
        if(err!=ErrorCodes::SUCCESS)
            {
            qDebug()<<"Login failed,err is "<<err;
            emit sig_login_failed(err);
            return;
        }
        //从chatserver的回包，读取登录用户信息并写入usermgr
        auto uid = jsonObj["uid"].toInt();
        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();

        auto user_info = std::make_shared<UserInfo>(uid,name,nick,icon,sex);
        UserMgr::GetInstance()->SetUserInfo(user_info);
        UserMgr::GetInstance()->SetToken(jsonObj["token"].toString());

        //从chatserver回包中读取向登录用户申请好友的列表
        if(jsonObj.contains("apply_list"))
            {
            UserMgr::GetInstance()->AppendApplyList(jsonObj["apply_list"].toArray());
        }

        //添加好友列表
        if(jsonObj.contains("friend_list"))
            {
            UserMgr::GetInstance()->AppendFriendList(jsonObj["friend_list"].toArray());
        }

        emit sig_switch_chatdlg();
    });

    //处理搜索好友请求回包
    _handlers.insert(ID_SEARCH_USER_RSP,[this](ReqId id,int len,QByteArray data){

        Q_UNUSED(len);
        qDebug()<<"handle id is "<<id<<"data is "<<data;
        //将QByteArray转为QJSONDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        //检查转换是否成功
        if(jsonDoc.isNull())
        {
            qDebug()<<"failed to create QJsonDocument";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error"))
        {
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"User Search Failed,err is Json Parse Err"<<err;
            emit sig_user_search(nullptr);//搜索好友失败
            return;
        }
        int err = jsonObj["error"].toInt();
        if(err!=ErrorCodes::SUCCESS)
        {
            qDebug()<<"User Search failed,err is "<<err;
            emit sig_user_search(nullptr);//搜索好友失败
            return;
        }

        auto search_info = std::make_shared<SearchInfo>(jsonObj["uid"].toInt(),
                                                      jsonObj["name"].toString(),
                                                      jsonObj["nick"].toString(),
                                                      jsonObj["desc"].toString(),
                                                      jsonObj["sex"].toInt(),
                                                      jsonObj["icon"].toString());

        //发送chserver传来的用户搜索结果
        emit sig_user_search(search_info);

    });

    //处理添加好友回包
    _handlers.insert(ID_ADD_FRIEND_RSP,[this](ReqId id,int len,QByteArray data){

        Q_UNUSED(len);
        qDebug()<<"handle id is "<<id<<"data is "<<data;
        //将QByteArray转为QJSONDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        //检查转换是否成功
        if(jsonDoc.isNull())
        {
            qDebug()<<"failed to create QJsonDocument";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error"))
        {
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Add Friend Apply(rsp) Failed,err is Json Parse Err"<<err;
            return;
        }
        int err = jsonObj["error"].toInt();
        if(err!=ErrorCodes::SUCCESS)
        {
            qDebug()<<"Add Friend Apply(rsp) failed,err is "<<err;
            return;
        }

        //打印一下添加好友申请已经发送并收到chatserver回复
        qDebug()<<"Add friend Request Success ";



    });

    //处理chatserver发来的通知申请好友请求
    _handlers.insert(ID_NOTIFY_ADD_FRIEND_REQ,[this](ReqId id,int len,QByteArray data){

        Q_UNUSED(len);
        qDebug()<<"handle id is "<<id<<"data is "<<data;
        //将QByteArray转为QJSONDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        //检查转换是否成功
        if(jsonDoc.isNull())
        {
            qDebug()<<"failed to create QJsonDocument";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error"))
        {
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Notify Add Friend Request Failed,err is Json Parse Err"<<err;
            return;
        }
        int err = jsonObj["error"].toInt();
        if(err!=ErrorCodes::SUCCESS)
        {
            qDebug()<<"Notify Add Friend Request Failed,err is "<<err;
            return;
        }

        //解析notify_add_friend_req
        int from_uid = jsonObj["applyuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString desc = jsonObj["desc"].toString();
        QString icon = jsonObj["icon"].toString();
        QString nick = jsonObj["nick"].toString();
        int sex = jsonObj["sex"].toInt();

        auto apply_info = std::make_shared<AddFriendApply>(
            from_uid,name,desc,icon,nick,sex);
        //尚未认证通过的信息加入unauth_items

        //发送好友申请信号
        emit sig_friend_apply(apply_info);
        qDebug()<<"ID_NOTIFY_ADD_FRIEND_REQ FRIEND REQ Success ";
    });

    //处理chatserver发来的通知对方同意认证好友请求
    _handlers.insert(ID_NOTIFY_AUTH_FRIEND_REQ,[this](ReqId id,int len,QByteArray data){

        Q_UNUSED(len);
        qDebug()<<"handle id is "<<id<<"data is "<<data;
        //将QByteArray转为QJSONDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        //检查转换是否成功
        if(jsonDoc.isNull())
        {
            qDebug()<<"failed to create QJsonDocument";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if(!jsonObj.contains("error"))
        {
            int err = ErrorCodes::ERR_JSON;
            qDebug()<<"Notify Auth Friend Request Failed,err is Json Parse Err"<<err;
            return;
        }
        int err = jsonObj["error"].toInt();
        if(err!=ErrorCodes::SUCCESS)
        {
            qDebug()<<"Notify Auth Friend Request Failed,err is "<<err;
            return;
        }

        //解析notify_auth_friend_req
        int from_uid = jsonObj["fromuid"].toInt();
        QString name = jsonObj["name"].toString();
        QString nick = jsonObj["nick"].toString();
        QString icon = jsonObj["icon"].toString();
        int sex = jsonObj["sex"].toInt();


        auto auth_info = std::make_shared<AuthInfo>(from_uid,name,
                                                    nick, icon, sex);//chatserver传送过来的认证用户信息

        emit sig_add_auth_friend(auth_info);

        qDebug()<<"ID_NOTIFY_Auth_FRIEND_REQ Friend REQ Success ";
    });

    //注册回调函数：client发送authfriend request之后server给client的回应消息
    _handlers.insert(ID_AUTH_FRIEND_RSP, [this](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "handle id is " << id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if (jsonDoc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Auth Friend Failed, err is Json Parse Err" << err;
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Auth Friend Failed, err is " << err;
            return;
        }

        //解析添加好友发起者的信息，被申请者准备在界面添加好友信息
        auto name = jsonObj["name"].toString();
        auto nick = jsonObj["nick"].toString();
        auto icon = jsonObj["icon"].toString();
        auto sex = jsonObj["sex"].toInt();
        auto uid = jsonObj["uid"].toInt();
        auto rsp = std::make_shared<AuthRsp>(uid, name, nick, icon, sex);
        emit sig_auth_rsp(rsp);

        qDebug() << "Auth Friend Success " ;
    });

}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto find_iter = _handlers.find(id);
    if(find_iter==_handlers.end())
    {
        qDebug()<<"not found id ["<<id<<"] to handle";
        return;
    }
    find_iter.value()(id,len,data);

}

void TcpMgr::slot_tcp_connect(ServerInfo si)
{
    qDebug()<< "receive tcp connect signal";
    // 尝试连接到服务器
    qDebug() << "Connecting to server...";
    _host = si.Host;
    _port = static_cast<uint16_t>(si.Port.toUInt());
    _socket.connectToHost(si.Host, _port);
}

void TcpMgr::slot_send_data(ReqId reqId, QByteArray data)
{
    //客户端发送数据可能在多个线程，保证数据安全，在发送数据时要发送sig_send_data信号，一下是对应槽函数的实现
    quint16 id = reqId;

    //计算字符串长度
    quint16 len = static_cast<quint16>(data.size());
    //存储所有字节流
    QByteArray block;
    //数据流
    QDataStream out(&block,QIODevice::WriteOnly);

    //数据流设为大端字节序
    out.setByteOrder(QDataStream::BigEndian);

    //写入id和长度
    out<<id<<len;
    //添加字符串数据
    block.append(data);

    //发送数据
    _socket.write(block);
    qDebug() << "tcp mgr send byte data is " << block ;

}





