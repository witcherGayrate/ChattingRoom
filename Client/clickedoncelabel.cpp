#include "clickedoncelabel.h"



ClickedOnceLabel::ClickedOnceLabel(QWidget *parent):QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}

void ClickedOnceLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked(this->text());
        return;
    }
    //基类继续处理
    QLabel::mouseReleaseEvent(event);
}
