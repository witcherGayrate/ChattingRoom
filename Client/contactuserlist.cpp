#include "contactuserlist.h"
#include"conuseritem.h"
#include"grouptipitem.h"
#include<QRandomGenerator>
#include"tcpmgr.h"
#include"usermgr.h"
#include<QTimer>
#include<QCoreApplication>
ContactUserList::ContactUserList(QWidget *parent):QListWidget(parent),_add_friend_item(nullptr),_load_pending(false)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //安装事件过滤器
    this->installEventFilter(this);

    //模拟从数据库或者后端传输过来的数据，进行列表加载
    addContactUserList();
    //连接点击item的信号和槽
    connect(this,&QListWidget::itemClicked,this,&ContactUserList::slot_item_clicked);

    //连接对端同意认证之后的信号，让对端刷新自己的联系人，聊天界面
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_add_auth_friend,this,&ContactUserList::slot_add_auth_friend);
    //同意对端的认证请求之后，刷新自己的联系人，聊天界面
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_auth_rsp,this,&ContactUserList::slot_auth_rsp);
    //to do 刷新申请好友列表
}

void ContactUserList::ShowRedPoint(bool bshow)
{
    _add_friend_item->ShowRedPoint(bshow);
}

void ContactUserList::addContactUserList()
{
    //加上groupitem
    auto groupTip = new GroupTipItem();//parent 设为userlist
    //原生的listwidgetitem,占位，用自定义的widget替换它
    QListWidgetItem* item = new QListWidgetItem();
    item->setSizeHint(groupTip->sizeHint());//设置item大小
    this->addItem(item);//向list中添加item
    this->setItemWidget(item,groupTip);//替换自定义的静态样式widget
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);//设置item的flags

    //添加好友申请的item
    _add_friend_item = new ConUserItem();
    QListWidgetItem* add_item = new QListWidgetItem();
    _add_friend_item->SetInfo(0,tr("新的朋友"),":/icons/add_friend.png");
    _add_friend_item->setObjectName("new_friend_item");
    _add_friend_item->SetItemType(ListItemType::APPLY_FRIEND_ITEM);
    add_item->setSizeHint(_add_friend_item->sizeHint());//根据conuseritem大小设置listitem大小
    this->addItem(add_item);
    this->setItemWidget(add_item,_add_friend_item);//自定义item替换原生item的widget静态样式
    //设置新的朋友申请条目被选中
    this->setCurrentItem(add_item);

    //加上groupitem
    auto groupCon = new GroupTipItem();//parent 设为userlist
    groupCon->SetGroupTip(tr("联系人"));
    //原生的listwidgetitem,占位，用自定义的widget替换它
    _groupitem = new QListWidgetItem();
    _groupitem->setSizeHint(groupCon->sizeHint());//设置item大小
    this->addItem(_groupitem);//向list中添加item
    this->setItemWidget(_groupitem,groupCon);//替换自定义的静态样式widget
    _groupitem->setFlags(_groupitem->flags() & ~Qt::ItemIsSelectable);//设置item的flags


    //加载后端发送过来的好友列表
    auto con_list = UserMgr::GetInstance()->GetConListPerPage();
    for(auto & con_ele : con_list){
        auto *con_user_wid = new ConUserItem();
        con_user_wid->SetInfo(con_ele->_uid,con_ele->_name, con_ele->_icon);
        QListWidgetItem *item = new QListWidgetItem;
        //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
        item->setSizeHint(con_user_wid->sizeHint());
        this->addItem(item);
        this->setItemWidget(item, con_user_wid);
    }
    UserMgr::GetInstance()->UpdateContactLoadedCount();

    //加上一些预定义的用户
    for (int var = 0; var < 13; ++var) {
        int randomValue = QRandomGenerator::global()->bounded(500);//生成0~99之间的随机整数
        //int str_i = randomValue%strs.size();
        int head_i = randomValue%heads.size();
        int name_i =randomValue%names.size();

        auto *con_user_wid = new ConUserItem();
        con_user_wid->SetInfo(0,names[name_i],heads[head_i]);
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(con_user_wid->sizeHint());
        addItem(item);
        setItemWidget(item,con_user_wid);
    }
}

bool ContactUserList::eventFilter(QObject *watched, QEvent *event)
{
    //显示或隐藏滚动条
    if(watched==viewport() && event->type()==QEvent::Enter)//鼠标进入userlist的范围
    {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }else if(watched==viewport() && event->type() == QEvent::Leave)
    {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    //鼠标滚轮滚动控制
    if(watched == viewport() && event->type() ==QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y()/8;//纵坐标转动的角度/8
        int numSteps = numDegrees/15;//调整步幅

        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-numSteps);//滚轮上为正，滚动条上为小，所以是减法

        //检查是否滚动底部
        QScrollBar *srcollBar = this->verticalScrollBar();
        int maxScrollBarValue = srcollBar->maximum();
        int currentValue  = srcollBar->value();
        if(maxScrollBarValue - currentValue <=0)
        {
            auto b_loaded = UserMgr::GetInstance()->IsLoadConFin();//应该是IsLoadConFin
            if(b_loaded){
                return true;
            }

            if(_load_pending){
                return true;
            }

            _load_pending = true;

            QTimer::singleShot(100, [this](){
                _load_pending = false;
                QCoreApplication::quit(); // 完成后退出应用程序
            });
            // 滚动到底部，加载新的联系人
            qDebug()<<"load more contact user";
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_contact_user();


        }
        return true;
    }

    return QListWidget::eventFilter(watched,event);//事件继续传递给父类
}

void ContactUserList::slot_item_clicked(QListWidgetItem *item)
{
    //首先利用UserList的方法itemWidget从原生item获取自定义的widget
    QWidget* wid = this->itemWidget(item);
    if(!wid)
    {
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }
    //将widget转为自定义的ListItemBase
    ListItemBase* customItem = qobject_cast<ListItemBase*>(wid);//object转换
    if(!customItem)
    {
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }
    //判断ItemBase的UserType，根据不同type进行处理
    auto itemType = customItem->GetItemType();
    if(itemType == ListItemType::INVALID_ITEM || itemType==ListItemType::GROUP_TIP_ITEM)
    {
        qDebug()<<"slot invalid item clicked ";
        return;
    }
    if(itemType == ListItemType::APPLY_FRIEND_ITEM)//如果类型为apply_friend_item
    {
        //创建对话框，提示用户
        qDebug()<<"apply friend item clicked";
        //跳转到好友申请界面
        emit sig_switch_apply_friend_page();
        return;
    }
    if(itemType == ListItemType::CONTACT_USER_ITEM)
    {
        //创建对话框，提示用户
        qDebug()<<"contact user item clicked";
        //跳转到好友信息界面
        emit sig_switch_friend_info_page();
        return;
    }
}

void ContactUserList::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info)
{

    qDebug() << "slot add auth friend ";
    //检查是否认证好友是否已经在好友列表
    bool isFriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if(isFriend){
        return;
    }
    // 在 groupitem 之后插入新项
    //int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    //int str_i = randomValue%strs.size();
    //int head_i = randomValue%heads.size();

    auto *con_user_wid = new ConUserItem();
    con_user_wid->SetInfo(auth_info);
    QListWidgetItem *item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(con_user_wid->sizeHint());

    // 获取 groupitem 的索引
    int index = this->row(_groupitem);
    // 在 groupitem 之后插入新项
    this->insertItem(index + 1, item);

    this->setItemWidget(item, con_user_wid);

}

void ContactUserList::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "slot auth rsp called";
    bool isFriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if(isFriend){
        return;
    }
    // 在 groupitem 之后插入新项
    //int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    //int str_i = randomValue%strs.size();
    //int head_i = randomValue%heads.size();

    auto *con_user_wid = new ConUserItem();
    con_user_wid->SetInfo(auth_rsp); //直接用auth_rsp构造
    QListWidgetItem *item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(con_user_wid->sizeHint());

    // 获取 groupitem 的索引
    int index = this->row(_groupitem);
    // 在 groupitem 之后插入新项
    this->insertItem(index + 1, item);

    this->setItemWidget(item, con_user_wid);

}
