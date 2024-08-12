#ifndef CONTACTUSERLIST_H
#define CONTACTUSERLIST_H

#include <QListWidget>
#include<QWheelEvent>
#include<QEvent>
#include<QScrollBar>
#include<QDebug>
#include<memory>
#include"userdata.h"

class ConUserItem;

class ContactUserList : public QListWidget
{
    Q_OBJECT
public:
    ContactUserList(QWidget* parent=nullptr);
    void ShowRedPoint(bool bshow=true);//在useritem上显示红点
private:
    void addContactUserList();//添加useritem
    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
public slots:
    void slot_item_clicked(QListWidgetItem* item);//list中的item被点击时
    void slot_add_auth_friend(std::shared_ptr<AuthInfo>);//别人通过申请时
    void slot_auth_rsp(std::shared_ptr<AuthRsp>);
signals:
    void sig_loading_contact_user();//加载联系人
    void sig_switch_apply_friend_page();//切换到申请好友页面
    void sig_switch_friend_info_page();//切换到好友信息页面
private:
    ConUserItem* _add_friend_item;
    QListWidgetItem* _groupitem;
    bool _load_pending;
};



#endif // CONTACTUSERLIST_H
