#ifndef APPLYFRIENDITEM_H
#define APPLYFRIENDITEM_H

#include <QWidget>
#include"listitembase.h"
#include"userdata.h"
#include<memory>
namespace Ui {
class ApplyFriendItem;
}

class ApplyFriendItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ApplyFriendItem(QWidget *parent = nullptr);
    ~ApplyFriendItem();
    void SetInfo(std::shared_ptr<ApplyInfo> apply_info);//存储applyinfo
    void ShowAddBtn(bool bshow);//是否展示添加按钮
    QSize sizeHint() const override//返回窗口大小
    {
        return QSize(250,80);
    }
    int GetUid();//获取uid
private:
    std::shared_ptr<ApplyInfo> _apply_info;//存储applyInfo
    bool _added;
    Ui::ApplyFriendItem *ui;
signals:
    void sig_auth_friend(std::shared_ptr<ApplyInfo> apply_info);//同意申请信息
};

#endif // APPLYFRIENDITEM_H
