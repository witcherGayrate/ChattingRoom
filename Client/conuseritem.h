#ifndef CONUSERITEM_H
#define CONUSERITEM_H
#include"userdata.h"
#include <QWidget>
#include"listitembase.h"
namespace Ui {
class ConUserItem;
}

class ConUserItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ConUserItem(QWidget *parent = nullptr);
    ~ConUserItem();
    QSize sizeHint() const override;//返回窗口大小
    void SetInfo(std::shared_ptr<AuthInfo> auth_info);
    void SetInfo(std::shared_ptr<AuthRsp> auth_rsp);
    void SetInfo(int uid,QString name,QString icon);
    void ShowRedPoint(bool show=false);//是否展示红点
private:
    Ui::ConUserItem *ui;
    std::shared_ptr<UserInfo> _info;//管理用户的信息
};

#endif // CONUSERITEM_H
