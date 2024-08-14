#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/****************************************************************
 *
 * @file        mainwindow.h
 * @brief       主窗口
 *
 * @author      胖胖红
 * @date        2024/05/30
 * @history
 ***************************************************************/
#include <QMainWindow>
#include<logindialog.h>
#include<registerdialog.h>
#include<resetdialog.h>
#include<chatdialog.h>
#include<tcpmgr.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginDialog *login_dlg=nullptr;//登录界面类
    RegisterDialog *register_dlg=nullptr;//注册界面类

    ResetDialog *_reset_dlg =nullptr;//重置密码界面类
    ChatDialog* _chat_dlg = nullptr;//登录界面类
public slots:
    void do_switchToReg();
    void do_switchToLogin();
    void SlotSwitchReset();//切换到resetDialog
    void SlotSwitchLogin2();//resetdialog to logindialog
    void SlotSwitchChat();//切换到聊天界面

};


#endif // MAINWINDOW_H
