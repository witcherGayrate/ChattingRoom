#include "chatuserwid.h"
#include "ui_chatuserwid.h"

ChatUserWid::ChatUserWid(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ChatUserWid)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
}

ChatUserWid::~ChatUserWid()
{
    delete ui;
}

std::shared_ptr<UserInfo> ChatUserWid::GetUserInfo()
{
    return _user_info;
}

void ChatUserWid::SetInfo(std::shared_ptr<UserInfo> user_info)
{
    _user_info = user_info;
    //加载图片
    QPixmap pixmap(_user_info->_icon);

    //设置图片自动缩放
    ui->icon_lab->setPixmap(pixmap.scaled(ui->icon_lab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lab->setScaledContents(true);

    ui->user_name_lab->setText(_user_info->_name);
    ui->user_chat_lab->setText(_user_info->_last_msg);

}


