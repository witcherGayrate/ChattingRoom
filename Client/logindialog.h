#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;

signals:
    void switchRegister(); //点击注册按钮，发射信号
    void switchReset();//点击忘记密码发送，重置信号
private slots:
    void slot_forget_pwd();//点击忘记密码发送对应的信号

};

#endif // LOGINDIALOG_H
