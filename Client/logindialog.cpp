#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    //点击注册，登录界面发射信号从而在mainwindow处理
    connect(ui->btn_logup,&QPushButton::clicked,this,&LoginDialog::switchRegister);
    ui->lab_forget->setCursor(Qt::PointingHandCursor);
    ui->lab_forget->SetState("normal","hover","","selected","selected_hover","");
    connect(ui->lab_forget,&ClickedLabel::clicked,this,&LoginDialog::slot_forget_pwd);
}

LoginDialog::~LoginDialog()
{
    qDebug()<< "destruct LoginDlg";
    delete ui;
}

void LoginDialog::slot_forget_pwd() //点击忘记密码发送对应的信号
{
    qDebug()<<"slot forget pwd";
    emit switchReset();//mainWindow接收
}
