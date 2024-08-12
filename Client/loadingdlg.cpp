#include "loadingdlg.h"
#include "ui_loadingdlg.h"
#include<QMovie>
LoadingDlg::LoadingDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadingDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint|Qt::WindowStaysOnTopHint);//设置dialog无边框
    //无背景颜色
    this->setAttribute(Qt::WA_TranslucentBackground);
    //设置窗口大小与父窗体同大小
    setFixedSize(parent->size());

    QMovie *movie = new QMovie(":/icons/loading.gif");//加载动画的资源文件
    ui->loading_lab->setMovie(movie);
    movie->start();

}

LoadingDlg::~LoadingDlg()
{
    delete ui;
}
