#include "clickedlabel.h"
#include<QMouseEvent>
ClickedLabel::ClickedLabel(QWidget *parent):QLabel(parent),_curstate(ClickLBState::Normal)
{

}

void ClickedLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(_curstate==ClickLBState::Normal)
        {
            qDebug()<<"PressEvent,change to selected press: "<<_selected_press;
            _curstate = ClickLBState::Selected; //设置当前ClickLBState
            setProperty("state",_selected_press);
            repolish(this);
            update();
        }else
        {
            qDebug()<<"PressEvent,change to normal press: "<<_normal_hover;
            _curstate = ClickLBState::Normal;
            setProperty("state",_normal_press);
            repolish(this);
            update();//重新刷新一下
        }
        return;
    }
    //调用基类的mousePressEvent保证正常的事件处理
    QLabel::mousePressEvent(event);
}

void ClickedLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(_curstate==ClickLBState::Normal)
        {
            qDebug()<<"ReleaseEvent,change to normal hover: "<<_normal_hover;
            //_curstate = ClickLBState::Normal; //设置当前ClickLBState
            setProperty("state",_normal_hover);
            repolish(this);
            update();
        }else
        {
            qDebug()<<"ReleaseEvent,change to select hover: "<<_selected_hover;
            //_curstate = ClickLBState::Normal;
            setProperty("state",_selected_hover);
            repolish(this);
            update();//重新刷新一下
        }
        //发送点击信号，调整lineEdit code模式
        emit clicked(this->text(),_curstate);
        return;
    }
    //调用基类的mouseReleaseEvent保证正常的事件处理
    QLabel::mouseReleaseEvent(event);//原来有错pressevent
}

void ClickedLabel::enterEvent(QEnterEvent *event)
{
    //处理鼠标悬停进入的逻辑
    if(_curstate == ClickLBState::Normal)
    {
        qDebug()<<"enter, change to normal hover: "<<_normal_hover;
        setProperty("state",_normal_hover);
        repolish(this);
        update();
    }else
    {
        qDebug()<<"enter, change to selected hover: "<<_selected_hover;
        setProperty("state",_selected_hover);
        repolish(this);
        update();
    }
    QLabel::enterEvent(event);
}

void ClickedLabel::leaveEvent(QEvent *event)
{
    //处理鼠标离开进入的逻辑
    if(_curstate == ClickLBState::Normal)
    {
        qDebug()<<"enter, change to normal hover: "<<_normal;
        setProperty("state",_normal);
        repolish(this);
        update();
    }else
    {
        qDebug()<<"leave, change to selected hover: "<<_selected;
        setProperty("state",_selected);
        repolish(this);
        update();
    }
    QLabel::leaveEvent(event);
}

void ClickedLabel::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal=normal;
    _normal_hover = hover;
    _normal_press = press;

    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    setProperty("state",normal);
    repolish(this);
}

ClickLBState ClickedLabel::GetCurState()
{
    return _curstate;
}

bool ClickedLabel::SetCurState(ClickLBState state)
{
    _curstate = state;
    if(_curstate==ClickLBState::Normal)
    {
        setProperty("state",_normal);
        repolish(this);
    }else if(_curstate==ClickLBState::Selected)
    {
        setProperty("state",_selected);
        repolish(this);
    }

    return true;
}

void ClickedLabel::ResetNormalState()
{
    _curstate = ClickLBState::Normal;
    setProperty("state",_normal);
    repolish(this);
}
