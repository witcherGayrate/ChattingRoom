#include "chatdialog.h"
#include "ui_chatdialog.h"
#include <QRandomGenerator>
#include"chatuserwid.h"
#include"loadingdlg.h"
#include<QMouseEvent>
#include"tcpmgr.h"
#include"usermgr.h"
#include"conuseritem.h"
ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , _mode(ChatUIMode::ChatMode),_state(ChatUIMode::ChatMode),_b_loading(false),_cur_chat_uid(0),ui(new Ui::ChatDialog)
{
    ui->setupUi(this);
    //初始化licked_btn
    ui->add_btn->SetState("normal","hover","press");
    ui->search_edit->SetMaxLength(15);
    //搜索栏添加搜索action
    QAction *searchAction = new QAction(ui->search_edit);//创建action
    searchAction->setIcon(QIcon(":/icons/search.png"));//设置action的icon
    ui->search_edit->addAction(searchAction,QLineEdit::LeadingPosition);//edit添加action
    ui->search_edit->setPlaceholderText("搜索");//edit默认显示文本
    //搜索栏添加清除action
    QAction *clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/icons/close_transparent.png"));//透明图标
    ui->search_edit->addAction(clearAction,QLineEdit::TrailingPosition);
    //当实际显示清除图标时，改为实际的清除图标
    connect(ui->search_edit,&QLineEdit::textChanged,[clearAction](const QString &text){//textChanged信号传递的值即当前文本内容QString
        if(!text.isEmpty())
        {
            clearAction->setIcon(QIcon(":/icons/close_search.png"));
        }
        else
        {
            clearAction->setIcon(QIcon(":/icons/close_transparent.png"));
        }
    });
    //连接清除动作的触发信号到槽函数，用于清除文本
    connect(clearAction,&QAction::triggered,[this,clearAction](){
        ui->search_edit->clear();//清除文本
        clearAction->setIcon(QIcon(":/icons/close_transparent.png"));
        ui->search_edit->clearFocus();
        //清除按钮被按下则不显示搜索框
        ShowSearch(false);
    });

    //滚轮到底会发送添加userinfo的信号，chatdialog捕获信号并播放加载动画、加载新的user
    connect(ui->chat_user_list,&ChatUserList::sig_loading_chat_user,this,&ChatDialog::slot_loading_chat_user);

    ShowSearch(false);
    //预定义的用户信息
    addChatUserList();
    //加载左侧头像
    QPixmap pixmap(UserMgr::GetInstance()->GetUserInfo()->_icon);
    ui->side_head_lab->setPixmap(pixmap.scaled(ui->side_head_lab->size(),Qt::KeepAspectRatio));
    ui->side_head_lab->setScaledContents(true);
    //把侧边栏的按钮加入chatdialog的lab_list从而进行lab间的交互
    AddLBGroup(ui->side_chat_lab);
    AddLBGroup(ui->side_contact_lab);
    //点击侧边栏切换
    connect(ui->side_chat_lab,&StateWidget::clicked,this,&ChatDialog::slot_side_chat);
    connect(ui->side_contact_lab,&StateWidget::clicked,this,&ChatDialog::slot_side_contact);

    //当搜索框变化时
    connect(ui->search_edit,&QLineEdit::textChanged,this,&ChatDialog::slot_text_changed);
    //检测鼠标点击位置判断是否要清空搜索框
    this->installEventFilter(this);

    //为search_list设置search_edit
    ui->search_list->SetSearchEdit(ui->search_edit);

    //连接申请好友的通知信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_friend_apply,this,&ChatDialog::slot_apply_friend);

    //连接认证添加好友通知信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_add_auth_friend,this,&ChatDialog::slot_add_auth_friend);

    //连接认证添加好友回包信号
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_auth_rsp,this,&ChatDialog::slot_auth_rsp);

    //连接search_list跳转聊天信号
    connect(ui->search_list,&SearchList::sig_jump_chat_item,this,&ChatDialog::slot_jump_chat_item);

    //连接加载联系人的信号和槽函数
    connect(ui->con_user_list,&ContactUserList::sig_loading_contact_user,this,&ChatDialog::slot_loading_contact_user);
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::addChatUserList()
{

    //先按照好友列表加载聊天记录，等以后客户端实现聊天记录数据库之后再按照最后信息排序
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if(find_iter != _chat_items_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateChatLoadedCount();
    }


    //初始化userinfor vector
    initUserInfoList();
    //创建QListWidgetItem并设置自定义的widget
    for(int i=0;i<13;i++)
    {
        //随机选取userinfo
        int randomValue = QRandomGenerator::global()->bounded(100);//生成0~99之间的随机整数
        int str_i = randomValue%_strs.size();
        int name_i = randomValue%_names.size();
        int head_i = randomValue%_heads.size();

        //创建chatuserwid并设置info
        auto *chat_user_wid = new ChatUserWid();
        auto user_info = std::make_shared<UserInfo>(0,names[name_i],
                                                    names[name_i],heads[head_i],0,strs[str_i]);//虚拟userinfo
        chat_user_wid->SetInfo(user_info);
        //创建QListwidgetItem并调整大小
        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(chat_user_wid->sizeHint());
        //向chat_user_list添加item和widget
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item,chat_user_wid);
    }
}

void ChatDialog::initUserInfoList()
{
    _strs = {
        "走到哪儿了？你是不是边走边玩手机！",
        "食堂门口见。",
        "什么时候发工资，我没衣服穿了。",
        "今天又胖了两斤",
        "牛皮！",
        "几点下班？三缺一。",
        "这天气能把我晒成化了",
        "你真是个可爱的小胖子",
        "想出去玩，不想上班！"
    };

    _names = {
        "爱吃的胖红",
        "胖奇不想动",
        "AAA犟驴红",
        "乌歪达不溜",
        "春风秋雨",
        "赫尔",
        "小马哥",
        "熬夜波比",
        "贪吃波比"
    };

    _heads={
        ":/icons/head_1.jpg",
        ":/icons/head_2.jpg",
        ":/icons/head_3.jpg",
        ":/icons/head_4.jpg",
        ":/icons/head_5.jpg",
        ":/icons/head_6.png",
        ":/icons/head_7.png",
        ":/icons/head_8.png",
        ":/icons/head_9.png",
        ":/icons/head_10.png",
        ":/icons/head_12.png",
        ":/icons/head_13.png",
    };
}

void ChatDialog::ShowSearch(bool bsearch)
{
    if(bsearch)
    {
        ui->con_user_list->hide();
        ui->chat_user_list->hide();
        ui->search_list->show();
        _mode = ChatUIMode::SearchMode;
    }else if(_state==ChatUIMode::ChatMode)
    {
        ui->search_list->hide();
        ui->con_user_list->hide();
        ui->chat_user_list->show();
        _mode = ChatUIMode::ChatMode;
    }
    else if(_state==ChatUIMode::ContactMode)
    {
        ui->search_list->hide();
        ui->chat_user_list->hide();
        ui->con_user_list->show();
        _mode = ChatUIMode::ContactMode;
    }
}

void ChatDialog::AddLBGroup(StateWidget *lb)
{
    _lb_list.push_back(lb);
}

void ChatDialog::slot_loading_chat_user()
{
    if(_b_loading)
    {
        return;
    }
    _b_loading = true;
    LoadingDlg* loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);//设为模态显示，此时用户不能与其他窗体进行交互
    loadingDialog->show();
    qDebug()<<"add new data to list...";
    loadMoreChatUser();

    //加载后关闭loadingDlg（为了显示加载动画）
    loadingDialog->deleteLater();

    _b_loading=false;
}

void ChatDialog::slot_side_chat()
{
    qDebug()<<"receive side chat clicked";
    ClearLabelState(ui->side_chat_lab);
    //切换右边的展示界面
    ui->stackedWidget->setCurrentWidget(ui->chat_page);
    _state = ChatUIMode::ChatMode;
    ShowSearch(false);
}

void ChatDialog::slot_side_contact()
{
    qDebug()<<"receive side contact clicked";
    ClearLabelState(ui->side_contact_lab);
    //切换右边的展示界面
    ui->stackedWidget->setCurrentWidget(ui->friend_apply_page);
    _state = ChatUIMode::ContactMode;
    ShowSearch(false);

}

void ChatDialog::slot_text_changed(const QString &str)
{
    if(!str.isEmpty())
        ShowSearch(true);
}

void ChatDialog::slot_loading_contact_user()
{
    qDebug() << "slot loading contact user";
    if(_b_loading){
        return;
    }

    _b_loading = true;
    LoadingDlg *loadingDialog = new LoadingDlg(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    qDebug() << "add new data to list.....";
    loadMoreConUser();
    // 加载完成后关闭对话框
    loadingDialog->deleteLater();

    _b_loading = false;

}

void ChatDialog::slot_apply_friend(std::shared_ptr<AddFriendApply> apply)
{
    qDebug()<<"receive apply friend slot, applyuid is "<<apply->_from_uid<< " name is "<< apply->_name
             <<" desc is "<<apply->_desc;

    bool b_already = UserMgr::GetInstance()->AlreadyApply(apply->_from_uid);
    if(b_already)
        return;
    UserMgr::GetInstance()->AddApplyList(std::make_shared<ApplyInfo>(apply));
    ui->side_contact_lab->ShowRedPoint(true);
    ui->con_user_list->ShowRedPoint(true);//联系人列表
    ui->friend_apply_page->AddNewApply(apply);//好友申请界面列表
}

void ChatDialog::slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info)
{
    qDebug() << "receive slot_add_auth__friend uid is " << auth_info->_uid
             << " name is " << auth_info->_name << " nick is " << auth_info->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_info->_uid);
    if(bfriend){
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_info);

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_info);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_info->_uid, item);


}

void ChatDialog::slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp)
{
    qDebug() << "receive slot_auth_rsp uid is " << auth_rsp->_uid
             << " name is " << auth_rsp->_name << " nick is " << auth_rsp->_nick;

    //判断如果已经是好友则跳过
    auto bfriend = UserMgr::GetInstance()->CheckFriendById(auth_rsp->_uid);
    if(bfriend){
        return;
    }

    UserMgr::GetInstance()->AddFriend(auth_rsp);
    //int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
    //int str_i = randomValue % strs.size();
    //int head_i = randomValue % heads.size();
    //int name_i = randomValue % names.size();

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(auth_rsp);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);
    _chat_items_added.insert(auth_rsp->_uid, item);

}

void ChatDialog::slot_jump_chat_item(std::shared_ptr<SearchInfo> si)
{
    qDebug() << "slot jump chat item " << Qt::endl;
    auto find_iter = _chat_items_added.find(si->_uid);
    if(find_iter != _chat_items_added.end()){
        qDebug() << "jump to chat item , uid is " << si->_uid;
        ui->chat_user_list->scrollToItem(find_iter.value());
        ui->side_chat_lab->SetSelected(true);
        SetSelectChatItem(si->_uid);
        //更新聊天界面信息
        SetSelectChatPage(si->_uid);
        slot_side_chat();
        return;
    }

    //如果没找到，则创建新的插入listwidget

    auto* chat_user_wid = new ChatUserWid();
    auto user_info = std::make_shared<UserInfo>(si);
    chat_user_wid->SetInfo(user_info);
    QListWidgetItem* item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(chat_user_wid->sizeHint());
    ui->chat_user_list->insertItem(0, item);
    ui->chat_user_list->setItemWidget(item, chat_user_wid);

    _chat_items_added.insert(si->_uid, item);

    ui->side_chat_lab->SetSelected(true);
    SetSelectChatItem(si->_uid);
    //更新聊天界面信息
    SetSelectChatPage(si->_uid);
    slot_side_chat();

}

void ChatDialog::SetSelectChatItem(int uid)
{
    if(ui->chat_user_list->count() <= 0){
        return;
    }

    if(uid == 0){
        ui->chat_user_list->setCurrentRow(0);
        QListWidgetItem *firstItem = ui->chat_user_list->item(0);
        if(!firstItem){
            return;
        }

        //转为widget
        QWidget *widget = ui->chat_user_list->itemWidget(firstItem);
        if(!widget){
            return;
        }

        auto con_item = qobject_cast<ChatUserWid*>(widget);
        if(!con_item){
            return;
        }

        _cur_chat_uid = con_item->GetUserInfo()->_uid;

        return;
    }

    auto find_iter = _chat_items_added.find(uid);
    if(find_iter == _chat_items_added.end()){
        qDebug() << "uid " <<uid<< " not found, set curent row 0";
        ui->chat_user_list->setCurrentRow(0);
        return;
    }

    ui->chat_user_list->setCurrentItem(find_iter.value());

    _cur_chat_uid = uid;

}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        handleGlobalMousePress(mouseEvent);
    }
    return QDialog::eventFilter(watched,event);
}

void ChatDialog::ClearLabelState(StateWidget *lb)
{
    for(auto ele:_lb_list)
    {
        if(ele==lb)
            continue;
        ele->ClearState();
    }

}

void ChatDialog::handleGlobalMousePress(QMouseEvent *event)
{
    if(_mode!=ChatUIMode::SearchMode)
    {
        return;
    }
    QPoint posInSearchList =(ui->search_list->mapFromGlobal(event->globalPosition())).toPoint();
    //判断点击位置是否在聊天列表的范围内
    if(!ui->search_list->rect().contains(posInSearchList))
    {
        //如果不在聊天列表内，清空输入框
        ui->search_edit->clear();
        ShowSearch(false);
    }
}

void ChatDialog::SetSelectChatPage(int uid)
{

}

void ChatDialog::loadMoreChatUser()
{
    auto friend_list = UserMgr::GetInstance()->GetChatListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto find_iter = _chat_items_added.find(friend_ele->_uid);
            if(find_iter != _chat_items_added.end()){
                continue;
            }
            auto *chat_user_wid = new ChatUserWid();
            auto user_info = std::make_shared<UserInfo>(friend_ele);
            chat_user_wid->SetInfo(user_info);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->chat_user_list->addItem(item);
            ui->chat_user_list->setItemWidget(item, chat_user_wid);
            _chat_items_added.insert(friend_ele->_uid, item);
        }
        //更新已加载条目
        UserMgr::GetInstance()->UpdateChatLoadedCount();
    }

}

void ChatDialog::loadMoreConUser()
{
    auto friend_list = UserMgr::GetInstance()->GetConListPerPage();
    if (friend_list.empty() == false) {
        for(auto & friend_ele : friend_list){
            auto *chat_user_wid = new ConUserItem();
            chat_user_wid->SetInfo(friend_ele->_uid,friend_ele->_name,
                                   friend_ele->_icon);
            QListWidgetItem *item = new QListWidgetItem;
            //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
            item->setSizeHint(chat_user_wid->sizeHint());
            ui->con_user_list->addItem(item);
            ui->con_user_list->setItemWidget(item, chat_user_wid);
        }

        //更新已加载条目
        UserMgr::GetInstance()->UpdateContactLoadedCount();
    }

}
