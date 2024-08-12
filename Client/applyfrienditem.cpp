#include "applyfrienditem.h"
#include "ui_applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent)
    : ListItemBase(parent),_added(false)
    , ui(new Ui::ApplyFriendItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::APPLY_FRIEND_ITEM);//设置item类别
    ui->addBtn->SetState("normal","hover","press");
    ui->addBtn->hide();//有好友申请时才显示
    //点击添加好友发送申请信号交给网络管理类
    connect(ui->addBtn,&ClickedBtn::clicked,[this]()
            {
        emit this->sig_auth_friend(_apply_info);
    });
}

ApplyFriendItem::~ApplyFriendItem()
{
    delete ui;
}

//根据好友申请信息设置applyfrienditem
void ApplyFriendItem::SetInfo(std::shared_ptr<ApplyInfo> apply_info)
{
    _apply_info  = apply_info;
    //设置头像
    QPixmap pix(_apply_info->_icon);
    ui->icon_lab->setPixmap(pix.scaled(ui->icon_lab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->icon_lab->setScaledContents(true);//图片可缩放
    //设置名称和描述
    ui->user_name_lab->setText(_apply_info->_name);
    ui->user_chat_lab->setText(_apply_info->_desc);
}

void ApplyFriendItem::ShowAddBtn(bool bshow)
{
    if(bshow)
    {
        ui->addBtn->show();
        ui->already_add_lab->hide();
        _added = false;
    }
    else
    {
        ui->addBtn->hide();
        ui->already_add_lab->show();
        _added = true;
    }
}

int ApplyFriendItem::GetUid()
{
    return _apply_info->_uid;
}
