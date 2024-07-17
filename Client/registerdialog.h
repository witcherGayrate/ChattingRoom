#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include"global.h"
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_btn_acquire_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);//处理注册模块完成信号的槽函数

    void on_btn_sure_clicked();

    void on_return_btn_clicked();

private:
    void initHttpHandlers();
    void showTip(QString str,bool ok);
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkVarifyValid();
    bool checkConfirmValid();
    void ChangeTipPage();
    void AddTipErr(TipErr te,QString tips);
    void DelTipErr(TipErr ts);
    QMap<TipErr,QString> _tip_errs;
    Ui::RegisterDialog *ui;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;//创建不同请求id与对应回调函数的映射表

    QTimer *_countdown_timer;
    int _countdown;
signals:
    void sigSwitchLogin();
};



#endif // REGISTERDIALOG_H
