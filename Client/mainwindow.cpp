#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //mainwindow icon
    this->setWindowIcon(QIcon(":/icons/wechat.png"));
    //this->setWindowFlag(Qt::FramelessWindowHint);
    login_dlg = new LoginDialog(this);
    login_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);//设置窗口自定义，无边框
    setCentralWidget(login_dlg);//设置居中
    //login_dlg->show();

    connect(login_dlg,&LoginDialog::switchRegister,this,&MainWindow::do_switchToReg);
    //连接登录界面忘记密码信号
    connect(login_dlg,&LoginDialog::switchReset,this,&MainWindow::SlotSwitchReset);
    //连接创建聊天界面信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_switch_chatdlg,this,&MainWindow::SlotSwitchChat);

    //测试用直接自己发送跳转到chatdlg的信号
    //emit TcpMgr::GetInstance()->sig_switch_chatdlg();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//切换到注册窗口
void MainWindow::do_switchToReg()
{
    //注册界面
    register_dlg = new RegisterDialog(this);//注册界面类,调用完直接回收了

    register_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);//设置窗口自定义，无边框
    //连接注册界面返回登录信号
    connect(register_dlg,&RegisterDialog::sigSwitchLogin,this,&MainWindow::do_switchToLogin);

    setCentralWidget(register_dlg);//将注册界面设为中心
    login_dlg->hide();//隐藏登录界面
    register_dlg->show();//展示注册界面
}

void MainWindow::do_switchToLogin()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    login_dlg = new LoginDialog(this);
    login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(login_dlg);
    register_dlg->hide();
    login_dlg->show();
    //连接登录界面注册信号
    connect(login_dlg, &LoginDialog::switchRegister, this, &MainWindow::do_switchToReg);
    //连接登录界面忘记密码信号
    connect(login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
    //创建resetDialog并设为centralwidget
    _reset_dlg = new ResetDialog(this);
    _reset_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_reset_dlg);

    login_dlg->hide();
    _reset_dlg->show();
    //注册返回登录信号和槽函数
    connect(_reset_dlg,&ResetDialog::switchLogin,this,&MainWindow::SlotSwitchLogin2);
}

//从重置界面返回登录界面
void MainWindow::SlotSwitchLogin2()
{
    //创建一个CentralWidget, 并将其设置为MainWindow的中心部件
    login_dlg = new LoginDialog(this);
    login_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(login_dlg);

    _reset_dlg->hide();
    login_dlg->show();

    //连接登录界面忘记密码信号
    connect(login_dlg, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
    //连接登录界面注册信号
    connect(login_dlg, &LoginDialog::switchRegister, this, &MainWindow::do_switchToReg);
}

void MainWindow::SlotSwitchChat()
{
    _chat_dlg = new ChatDialog();
    _chat_dlg->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(_chat_dlg);
    _chat_dlg->show();
    login_dlg->hide();
    //调整主窗体大小
    this->setMinimumSize(QSize(1050,900));
    this->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
}
