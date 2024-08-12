#ifndef APPLYFRIENDPAGE_H
#define APPLYFRIENDPAGE_H
#include"userdata.h"
#include<memory>
#include<QJsonArray>
#include<unordered_map>
#include <QWidget>
#include"applyfrienditem.h"
namespace Ui {
class ApplyFriendPage;
}

class ApplyFriendPage : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyFriendPage(QWidget *parent = nullptr);
    ~ApplyFriendPage();
    void AddNewApply(std::shared_ptr<AddFriendApply>apply);//添加新的申请
protected:
    void paintEvent(QPaintEvent *event);//重写widget的喷绘事件
private:
    void loadApplyList();//初始化加载好友列表
    std::unordered_map<int,ApplyFriendItem*> _unauth_items; //申请的好友
public slots:
    void slot_auth_rsp(std::shared_ptr<AuthRsp>);//发送同意申请的消息
signals:
    void sig_show_search(bool);//展示搜索结果的信号

private:
    Ui::ApplyFriendPage *ui;
};

#endif // APPLYFRIENDPAGE_H
