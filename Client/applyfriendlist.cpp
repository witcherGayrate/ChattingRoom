#include "applyfriendlist.h"
//#include"listitembase.h"
#include<QScrollBar>
#include<QWheelEvent>
ApplyFriendList::ApplyFriendList(QWidget* parent):QListWidget(parent)
{
    //隐藏水平滚轮
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //安装事件过滤器
    this->viewport()->installEventFilter(this);
}

bool ApplyFriendList::eventFilter(QObject *watched, QEvent *event)
{
    //鼠标进入就显示滚动条，否则隐藏滚动条
    if(watched==viewport() &&event->type()==QEvent::Enter)
    {
        qDebug()<<"mouse enter";
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    }else if(watched==viewport()&& event->type()==QEvent::Leave)
    {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    if(watched==viewport() &&event->type()==QEvent::MouseButtonPress)
    {
        emit sig_show_search(false);//不显示搜索框
    }

    //检查事件是否是鼠标滚轮事件
    if(watched==viewport() && event->type()== QEvent::Wheel)
    {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        //滚动角度
        int numDegrees = wheelEvent->angleDelta().y()/8;
        //设置滚动幅度
        int numSteps = numDegrees/15;

        //设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value()-numSteps);
        return true;//停止事件传递
    }
    //继续交给基类处理
    return QListWidget::eventFilter(watched,event);
}
