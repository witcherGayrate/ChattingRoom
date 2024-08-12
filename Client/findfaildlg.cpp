#include "findfaildlg.h"
#include "ui_findfaildlg.h"
#include<QDebug>
FindFailDlg::FindFailDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindFailDlg)
{
    ui->setupUi(this);
    setWindowTitle("添加");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);//窗体无边框
    this->setObjectName("FindFailDlg");//设置对象名方便美化
    ui->fail_sure_btn->SetState("normal","hover","press");
    this->setModal(true);//模态对话框
}

FindFailDlg::~FindFailDlg()
{
    qDebug()<<"Find FailDlg destruct";
    delete ui;
}

void FindFailDlg::on_fail_sure_btn_clicked()
{
    this->hide();
}

