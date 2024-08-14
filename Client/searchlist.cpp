#include "searchlist.h"
#include"tcpmgr.h"
#include<QScrollBar>
#include"adduseritem.h"
#include"findsuccessdlg.h"
#include"userdata.h"
#include<memory>
#include"customlineedit.h"
#include"findfaildlg.h"
#include"usermgr.h"
SearchList::SearchList(QWidget *parent):QListWidget(parent),
    _send_pending(false),_find_dlg(nullptr),_search_edit(nullptr)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //安装事件过滤器
    this->viewport()->installEventFilter(this);

    //连接点击listitem时的信号和槽
    connect(this,&QListWidget::itemClicked,this,&SearchList::slot_item_clicked);
    //添加条目
    addTipItem();
    //连接搜索条目
    //连接搜索条目
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_user_search, this, &SearchList::slot_user_search);
}

void SearchList::CloseFindDlg()
{
    if(_find_dlg)
    {
        _find_dlg->hide();
        _find_dlg = nullptr;
    }
}

void SearchList::SetSearchEdit(QWidget *edit)
{
    _search_edit = edit;
}

bool SearchList::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==this->viewport() && event->type()==QEvent::Enter)
    {
        //鼠标悬浮，显示滚动条
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }else if(watched==this->viewport() && event->type()==QEvent::Leave)
    {
        //鼠标离开，隐藏滚动条
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    //滚轮
    if(watched==this->viewport() && event->type()==QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y()/8;
        int numSteps = numDegrees/15;//滚动步数

        //设置滚动步数
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        return true;

    }
    return QListWidget::eventFilter(watched, event);
}

void SearchList::waitPending(bool pending)
{
    //正在发送查询消息，需要窗口等待
    if(pending)
    {
        _loadingDialog = new LoadingDlg(this);
        _loadingDialog->setModal(true);
        _loadingDialog->show();
        _send_pending=pending;
    }else{
        _loadingDialog->hide();
        _loadingDialog->deleteLater();
        _send_pending = pending;
    }
}

void SearchList::addTipItem()
{
    auto *invalid_item = new QWidget();
    QListWidgetItem *item_tmp = new QListWidgetItem();
    invalid_item->setObjectName("invalid_item");
    item_tmp->setSizeHint(QSize(250,10));
    this->addItem(item_tmp);
    this->setItemWidget(item_tmp,invalid_item);//更换原来listwidgetitem的样式
    item_tmp->setFlags(item_tmp->flags() & ~Qt::ItemIsSelectable);//设为不可选状态

    //添加用户useritem
    auto *add_user_item = new AddUserItem();
    QListWidgetItem *item = new QListWidgetItem;
    //qDebug()<<"chat_user_wid sizeHint is " << chat_user_wid->sizeHint();
    item->setSizeHint(add_user_item->sizeHint());
    this->addItem(item);
    this->setItemWidget(item, add_user_item);
}

void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget* widget = this->itemWidget(item);
    if(widget==nullptr)
    {
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }

    //对自定义widget进行操作，将item转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem)
    {
        qDebug()<<"slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType==ListItemType::INVALID_ITEM)
    {
        qDebug()<<"slot invalid item clicked";
        return;
    }

    if(itemType == ListItemType::ADD_USER_TIP_ITEM)
    {
        //如果已经在发送请求消息了，就直接返回
        if(_send_pending)
        {
            return;
        }

        //检查指针
        if(!_search_edit)
        {
            return;
        }
        waitPending(true);
        //获取用户搜素框的文本，并写入json
        auto search_edit = dynamic_cast<CustomLineEdit*>(_search_edit);
        auto uid_str = search_edit->text();
        QJsonObject jsonObj;
        jsonObj["uid"] = uid_str;

        QJsonDocument doc(jsonObj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);//压缩json document
        //发送tcp消息给chatserver
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ,
                                                  jsonData);//优化sig_send_data

        return;
    }
    //清除弹出框
    CloseFindDlg();
}

void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    waitPending(false);
    if(si==nullptr)
    {
        //没有找到就弹出失败的对话框
        _find_dlg = std::make_shared<FindFailDlg>(this);
    }else
    {
        //此处分两种情况，一种是搜多到已经是自己的朋友了，一种是未添加好友
        //如果是自己，暂且直接返回，以后看逻辑扩充
        auto self_uid = UserMgr::GetInstance()->GetUid();
        if(self_uid==si->_uid)
        {
            return;
        }
        //查找是否已经是自己的朋友了
        bool bExist = UserMgr::GetInstance()->CheckFriendById(si->_uid);
        if(bExist)
        {
            //此处处理已经添加的好友，实现页面跳转
            //跳转到聊天界面指定的item中
            emit sig_jump_chat_item(si);
            return;
        }
        //此处先处理为添加的好友
        _find_dlg = std::make_shared<FindSuccessDlg>(this);//基类指针指向派生类对象
        std::dynamic_pointer_cast<FindSuccessDlg>(_find_dlg)->SetSearchInfo(si);//基类强转为派生类并设置搜索到的信息
        //此处处理已经添加的好友，实现页面跳转
    }
    _find_dlg->show();
}
