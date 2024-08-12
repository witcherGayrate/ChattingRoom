#include "logindialog.h"
#include "ui_logindialog.h"
#include"httpmgr.h"
#include"tcpmgr.h"
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    //点击注册，登录界面发射信号从而在mainwindow处理
    connect(ui->btn_logup,&QPushButton::clicked,this,&LoginDialog::switchRegister);

    //忘记密码动态效果
    ui->lab_forget->setCursor(Qt::PointingHandCursor);
    //ui->lab_forget->SetState("normal","hover","press","normal","hover","press");
    connect(ui->lab_forget,&ClickedLabel::clicked,this,&LoginDialog::slot_forget_pwd);

    //初始化错误提示
    ui->lab_tip->setProperty("state","normal");//设置lab_tip的状态
    repolish(ui->lab_tip);//刷新状态
    //设置大头照
    QPixmap headPhtoo(":/icons/head_3.jpg");
    headPhtoo = headPhtoo.scaled(ui->lab_photo->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->lab_photo->setPixmap(headPhtoo);


    //输入密码模式切换
    ui->lineEdit_pass->setEchoMode(QLineEdit::Password);
    ui->pwd_visible->setCursor(Qt::PointingHandCursor);
    ui->pwd_visible->SetState("unvisible","unvisible_hover","visible_hover","visible",
                              "visible_hover","visible_hover");
    //连接clickedlab的回调函数
    connect(ui->pwd_visible,&ClickedLabel::clicked,this,[this](){
        auto state = ui->pwd_visible->GetCurState();
        if(state==ClickLBState::Normal)
        {
            ui->lineEdit_pass->setEchoMode(QLineEdit::Password);
        }else
        {
            ui->lineEdit_pass->setEchoMode(QLineEdit::Normal);
        }
        qDebug()<<"Label was clicked!";
    });

    initHttpHandlers();
    //连接登录回包信号
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_login_mod_finish,this,&LoginDialog::slot_login_mod_finish);
    //连接tcp连接请求的信号和槽函数
    connect(this,&LoginDialog::sig_connect_tcp,TcpMgr::GetInstance().get(),&TcpMgr::slot_tcp_connect);
    //tcp连接结果的反馈
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_con_success,this,&LoginDialog::slot_tcp_con_finish);
    //连接tcp管理者发出登录失败信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_login_failed,this,&LoginDialog::slot_login_failed);

}

void LoginDialog::showTip(QString str, bool ok)
{
    if(ok==true)
    {
        ui->lab_tip->setProperty("state","normal");
    }else
    {
        ui->lab_tip->setProperty("state","err");
    }
    ui->lab_tip->setText(str);
    repolish(ui->lab_tip);
}

bool LoginDialog::checkUserValid()
{
    auto user = ui->lineEdit_user->text();
    if(user.isEmpty()){
        qDebug() << "User empty " ;
        showTip(tr("user empty!"),false);
        return false;
    }
    return true;
}

bool LoginDialog::checkPwdValid()
{
    auto pwd = ui->lineEdit_pass->text();
    if(pwd=="")
    {
        qDebug()<<"password line edit is null";
        showTip(tr("password is empty"),false);
    }
    if(pwd.length() < 6 || pwd.length() > 15){
        qDebug() << "Pass length invalid";
        showTip(tr("pass length invalid"),false);
        return false;
    }
    return true;
}

LoginDialog::~LoginDialog()
{
    qDebug()<< "destruct LoginDlg";
    delete ui;
}

void LoginDialog::initHttpHandlers()
{
    //注册获取登录回包逻辑
    _handlers.insert(ReqId::ID_LOGIN_USER,[this](QJsonObject jsonobj)
    {
        int error = jsonobj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"),false);
            return;
        }
        auto user = jsonobj["user"].toString();

        //发送信号通知tcpmgr发送长连接
        ServerInfo si;
        si.Uid = jsonobj["uid"].toInt();
        si.Host = jsonobj["host"].toString();
        si.Port = jsonobj["port"].toString();
        si.Token = jsonobj["token"].toString();

        _uid = si.Uid;
        _token = si.Token;
        qDebug()<< "user is " << user << " uid is " << si.Uid <<" host is "
                 << si.Host << " port is " << si.Port << " Token is " << si.Token;
        //发送信号并传递参数
        emit sig_connect_tcp(si);
    });
}

bool LoginDialog::enableBtn(bool enabled)
{
    ui->btn_login->setEnabled(enabled);
    ui->btn_logup->setEnabled(enabled);
    return true;
}

void LoginDialog::slot_forget_pwd() //点击忘记密码发送对应的信号
{
    qDebug()<<"slot forget pwd";
    emit switchReset();//mainWindow接收
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err!=ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"),false);
        return;
    }
    //解析Json字符串，res需转换为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //json解析错误
    if(jsonDoc.isNull())
    {
        showTip(tr("json解析错误"),false);
        return;
    }
    //json解析错误
    if(!jsonDoc.isObject()){
        showTip(tr("json解析错误"),false);
        return;
    }

    //调用对应的逻辑,根据id回调。
    _handlers[id](jsonDoc.object());
    return;
}

void LoginDialog::on_btn_login_clicked()
{
    qDebug()<<"login btn clicked";
    if(checkUserValid()==false)
    {
        return;
    }
    if(checkPwdValid()==false)
    {
        return;
    }

    auto user= ui->lineEdit_user->text();
    auto pwd = ui->lineEdit_pass->text();
    //发送http登录请求,用email进行登录
    QJsonObject json_obj;
    json_obj["email"] = user;
    json_obj["passwd"] = xorString(pwd);//加密字符串

    //qDebug()<<"email : "<<user;
    //qDebug()<<"passward: "<<xorString(pwd);

    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                                        json_obj,ReqId::ID_LOGIN_USER,Modules::LOGINMOD);

}

void LoginDialog::slot_tcp_con_finish(bool bsuccess)
{
    if(bsuccess)
    {
        showTip(tr("聊天服务连接成功，正在登录..."),true);
        QJsonObject jsonObj;
        jsonObj["uid"] = _uid;
        jsonObj["token"] = _token;
        QJsonDocument doc(jsonObj);
        QByteArray jsonString = doc.toJson(QJsonDocument::Indented);
        //发送tcp请求给chatserver
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN,jsonString);

    }else
    {
        showTip(tr("网络异常"),false);
        enableBtn(true);
    }
}

void LoginDialog::slot_login_failed(int err)
{
    QString result = QString("登录失败, err is %1")
                         .arg(err);
    showTip(result,false);
    enableBtn(true);
}

