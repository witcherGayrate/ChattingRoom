#include "applyfriendpage.h"
#include "authenfriend.h"
#include "ui_applyfriendpage.h"
#include"tcpmgr.h"
#include<QRandomGenerator>
#include<QStyleOption>
#include<QPainter>
#include"usermgr.h"
ApplyFriendPage::ApplyFriendPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApplyFriendPage)
{
    ui->setupUi(this);
    //传递隐藏搜索框的信号
    connect(ui->apply_friend_list,&ApplyFriendList::sig_show_search,this,&ApplyFriendPage::sig_show_search);
    //初始化加载用户列表
    loadApplyList();
    //接受tcp传递的authrsp信号处理
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_auth_rsp,this,&ApplyFriendPage::slot_auth_rsp);

}

ApplyFriendPage::~ApplyFriendPage()
{
    delete ui;
}

void ApplyFriendPage::AddNewApply(std::shared_ptr<AddFriendApply> apply)
{
    //将AddFriendApply转为ApplyInfo读取申请信息
    auto apply_item = new ApplyFriendItem(); 
    apply_item->SetInfo(std::make_shared<ApplyInfo>(apply));//存储好友申请item的申请信息
    //把申请信息加入到未认证用户信息
    _unauth_items[apply->_from_uid]= apply_item;
    //创建原生QListWidget，用自定义的item（QWidget）替换样式
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(apply_item->sizeHint());//设置大小
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);//设置item的标记
    ui->apply_friend_list->insertItem(0,item);//从上面添加
    ui->apply_friend_list->setItemWidget(item,apply_item);//替换原有样式
    apply_item->ShowAddBtn(true);

    //绑定审核好友的信号
    connect(apply_item,&ApplyFriendItem::sig_auth_friend,[this](std::shared_ptr<ApplyInfo>apply_info){
        auto* authFriend = new AuthenFriend(this);
        authFriend->setModal(true);
        authFriend->SetApplyInfo(apply_info);
        authFriend->show();
    });
}

void ApplyFriendPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ApplyFriendPage::loadApplyList()
{
    //获取好友申请
    auto apply_list = UserMgr::GetInstance()->GetApplyList();
    for(auto &apply:apply_list)
    {
        auto* apply_item = new ApplyFriendItem();
        apply_item->SetInfo(apply);
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        ui->apply_friend_list->insertItem(0,item);
        ui->apply_friend_list->setItemWidget(item,apply_item);
        if(apply->_status)
        {
            apply_item->ShowAddBtn(false);//该apply_info已经添加
        }else{
            apply_item->ShowAddBtn(true);
            auto uid = apply_item->GetUid();
            _unauth_items[uid] = apply_item;//该好友申请未经同意
        }

        //绑定好友申请信号
        connect(apply_item,&ApplyFriendItem::sig_auth_friend,[this](std::shared_ptr<ApplyInfo> apply_info)
                {
            auto *authFriend = new AuthenFriend(this);
            authFriend->setModal(true);
            authFriend->SetApplyInfo(apply_info);
            authFriend->show();
        });

    }

    //模拟假数据，创建QListWidgetItem,并设置自定义的widget
    for(int i=0;i<13;i++)
    {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();

        auto *apply_item = new ApplyFriendItem();
        auto apply = std::make_shared<ApplyInfo>(0, names[name_i], strs[str_i],
                                                 heads[head_i], names[name_i], 0, 1);
        apply_item->SetInfo(apply);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(apply_item->sizeHint());
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
        ui->apply_friend_list->addItem(item);
        ui->apply_friend_list->setItemWidget(item, apply_item);

        //绑定好友申请信号
        connect(apply_item, &ApplyFriendItem::sig_auth_friend, [this](std::shared_ptr<ApplyInfo> apply_info){
            auto *authFriend =  new AuthenFriend(this);
            authFriend->setModal(true);
            authFriend->SetApplyInfo(apply_info);
            authFriend->show();
        });
    }

}

void ApplyFriendPage::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    auto uid = auth_rsp->_uid;
    auto find_iter = _unauth_items.find(uid);
    if (find_iter == _unauth_items.end()) {
        qDebug()<<"------ApplyFriendPage::slot_auth_rsp: find_iter == _unauth_items.end()--return;";
        return;
    }

    find_iter->second->ShowAddBtn(false);
    qDebug()<<"------ApplyFriendPage::slot_auth_rsp: find_iter->second->ShowAddBtn(false);";
}
