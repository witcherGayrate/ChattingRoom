#include "clickedbtn.h"
#include"global.h"
#include<QVariant>
ClickedBtn::ClickedBtn(QWidget *parent):QPushButton(parent)
{
    setCursor(Qt::PointingHandCursor);//设置光标为手
    setFocusPolicy(Qt::NoFocus);//设置光标无focus，enter不触发按钮
}

ClickedBtn::~ClickedBtn()
{

}

void ClickedBtn::SetState(QString normal, QString hover, QString press)
{
    _normal= normal;
    _hover = hover;
    _press = press;
    //设置clickedBtn的属性
    setProperty("state","normal");
    repolish(this);
    update();//刷新一下样式
}

void ClickedBtn::mousePressEvent(QMouseEvent *event)
{
    setProperty("state",_press);
    repolish(this);
    update();
    QPushButton::mousePressEvent(event);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent *event)
{
    setProperty("state",_hover);
    repolish(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}

void ClickedBtn::leaveEvent(QEvent *event)
{
    setProperty("state",_normal);
    repolish(this);
    update();
    QPushButton::leaveEvent(event);
}

void ClickedBtn::enterEvent(QEnterEvent *event)
{
    setProperty("state",_hover);
    repolish(this);
    update();
    //继续交给基类处理
    QPushButton::enterEvent(event);
}

