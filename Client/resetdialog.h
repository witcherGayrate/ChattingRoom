#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include"global.h"
#include"httpmgr.h"
#include<QDebug>
#include<QRegularExpression>

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();


private slots:
    void on_btn_verifycode_clicked();
    //返回的id调用不同的回包处理逻辑
    void slot_reset_mod_finish(ReqId id,QString res,ErrorCodes err);

    void on_btn_sure_clicked();

    void on_btn_return_clicked();

private:
    Ui::ResetDialog *ui;
    //Http回包处理初始化
    void initHandlers();
    //错误提示
    void showTip(QString str,bool ok);

    //检测输入错误
    bool checkUserValid();
    bool checkPassValid();
    bool checkEmailValid();
    bool checkVerifyValid();

    //添加错误
    void AddTipErr(TipErr te,QString tips);
    //删除错误
    void DelTipErr(TipErr te);

    //回包逻辑,不同请求id与对应回调函数的映射表
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
    //输入错误map
    QMap<TipErr,QString> _tip_errs;
signals:
    void switchLogin();
};

#endif // RESETDIALOG_H
