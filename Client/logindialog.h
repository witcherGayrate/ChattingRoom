#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include<QJsonObject>
#include"global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    void showTip(QString str,bool ok);
    bool checkUserValid();
    bool checkPwdValid();

    ~LoginDialog();

private:
    //http回包的回调函数map表
    QMap<ReqId,std::function<void(QJsonObject)>> _handlers;
    //注册回调函数
    void initHttpHandlers();
    bool enableBtn(bool);
    int _uid;
    QString _token;
    Ui::LoginDialog *ui;
signals:
    void switchRegister(); //点击注册按钮，发射信号
    void switchReset();//点击忘记密码发送，重置信号
    void sig_connect_tcp(ServerInfo);//验证成功之后发送tcp连接的信号
private slots:
    void slot_forget_pwd();//点击忘记密码发送对应的信号
    void slot_login_mod_finish(ReqId id,QString res,ErrorCodes err);//Http登录回包处理
    void on_btn_login_clicked();
    void slot_tcp_con_finish(bool bsuccess);//logindialog收到tcp连接成功的回调
    void slot_login_failed(int err);
};

#endif // LOGINDIALOG_H
