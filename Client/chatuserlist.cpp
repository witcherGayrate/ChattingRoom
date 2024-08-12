#include "chatuserlist.h"
#include"usermgr.h"
#include<QTimer>
#include<QCoreApplication>
ChatUserList::ChatUserList(QWidget *parent):QListWidget(parent),_load_pending(false)
{
    Q_UNUSED(parent);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //按照事件过滤器
    this->viewport()->installEventFilter(this);
}

//widget组件没有event的虚函数时，需要用到事件过滤器自定义事件处理
bool ChatUserList::eventFilter(QObject *watched, QEvent *event)
{
    //鼠标进入或离开被监视对象做相应处理
    if(watched == this->viewport())
    {
        if(event->type() == QEvent::Enter)
        {
            //鼠标进入，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }else if(event->type()==QEvent::Leave)
        {
            //鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    if(watched==this->viewport() && event->type()==QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y()/8;//滚轮在y轴上滚动角度/8
        int numSteps = numDegrees/15;//滚动步数=滚动角度/8

        //设置滚动度数
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-numSteps);

        //检查是否滚动到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();
        //int pageSize = 10;//每页可加载的联系人数量

        if(maxScrollValue-currentValue<=0)
        {
            auto b_loaded = UserMgr::GetInstance()->IsLoadChatFin();
            if(b_loaded){
                return true;
            }

            if(_load_pending){
                return true;
            }
            // 滚动到底部，加载新的联系人
            qDebug()<<"load more chat user";
            _load_pending = true;

            QTimer::singleShot(100, [this](){
                _load_pending = false;
                QCoreApplication::quit(); // 完成后退出应用程序
            });
            //发送信号通知聊天界面加载更多聊天内容
            emit sig_loading_chat_user();

        }
        return true;//停止事件传递
    }
    //继续其他处理交给基类
    return QListWidget::eventFilter(watched,event);
}
