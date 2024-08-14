#include "conuseritem.h"
#include "ui_conuseritem.h"

ConUserItem::ConUserItem(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ConUserItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CONTACT_USER_ITEM);//设置类别

    ui->red_point->raise();//红点图标位于上层
    ShowRedPoint(false);
}


ConUserItem::~ConUserItem()
{
    delete ui;
}

QSize ConUserItem::sizeHint() const
{
    return QSize(250,70);//返回自定义的大小
}

void ConUserItem::SetInfo(std::shared_ptr<AuthInfo> auth_info)
{
    _info = std::make_shared<UserInfo>(auth_info);//调用UserInfo的构造函数
    //加载图片
    QPixmap pixmap(_info->_icon);

    //设置图片自动缩放
    ui->icon_lab->setPixmap(pixmap.scaled(ui->icon_lab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lab->setScaledContents(true);
    //设置名字
    ui->user_name_lab->setText(_info->_name);

}

void ConUserItem::SetInfo(std::shared_ptr<AuthRsp> auth_rsp)
{
    _info = std::make_shared<UserInfo>(auth_rsp);//调用UserInfo的构造函数
    //加载图片
    QPixmap pixmap(_info->_icon);

    //设置图片自动缩放
    ui->icon_lab->setPixmap(pixmap.scaled(ui->icon_lab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lab->setScaledContents(true);
    //设置名字
    ui->user_name_lab->setText(_info->_name);
}

void ConUserItem::SetInfo(int uid, QString name, QString icon)
{

    _info = std::make_shared<UserInfo>(uid,name,icon);
    //加载图片
    QPixmap pixmap(_info->_icon);

    //设置图片自动缩放
    ui->icon_lab->setPixmap(pixmap.scaled(ui->icon_lab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lab->setScaledContents(true);
    //设置名字
    ui->user_name_lab->setText(_info->_name);
}

std::shared_ptr<UserInfo> ConUserItem::GetInfo()
{
    return _info;
}

void ConUserItem::ShowRedPoint(bool show)
{
    if(show)
    {
        ui->red_point->show();
    }else
    {
        ui->red_point->hide();
    }
}


