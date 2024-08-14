#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include"listitembase.h"
#include"userdata.h"
namespace Ui {
class ChatUserWid;
}

class ChatUserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWid(QWidget *parent = nullptr);
    ~ChatUserWid();
    //设置大小
    QSize sizeHint() const override{return QSize(250,70);}
    std::shared_ptr<UserInfo> GetUserInfo();
    void SetInfo(std::shared_ptr<UserInfo> user_info);
    void SetInfo(std::shared_ptr<FriendInfo> friend_info);
    void updataLastMsg(std::vector<std::shared_ptr<TextChatData>> msgs);
private:
    Ui::ChatUserWid *ui;

    std::shared_ptr<UserInfo> _user_info;
};

#endif // CHATUSERWID_H
