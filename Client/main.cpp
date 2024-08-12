#include "mainwindow.h"
#include<QFile>
#include<QDir>
#include<QSettings>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QFile qss(":/qss/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug()<<"qss open sucess";
        QString style = QLatin1String(qss.readAll());//bytearray转为string
        a.setStyleSheet(style);
        qss.close();//用完关闭
    }
    else
    {
        qDebug()<<"open failed";
    }

    //获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    //拼接文件名
    QString fileName = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path+
                                                   QDir::separator()+fileName);
    QSettings settings(config_path,QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port  = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://"+gate_host+":"+gate_port;
    qDebug()<<gate_url_prefix;
    MainWindow w;
    w.show();
    return a.exec();
}
