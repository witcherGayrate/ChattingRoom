#include "picturebubble.h"
#include<QLabel>
#include<QPixmap>
#define PIC_MAX_WIDTH 160
#define PIC_MAX_HEIGHT 90
PictureBubble::PictureBubble(const QPixmap &picture, ChatRole role, QWidget *parent):
    BubbleFrame(role,parent)
{
    QLabel* lb = new QLabel();
    lb->setScaledContents(true);
    QPixmap pix = picture.scaled(QSize(PIC_MAX_HEIGHT,PIC_MAX_WIDTH),Qt::KeepAspectRatio);
    lb->setPixmap(pix);
    this->setWidget(lb);//添加到bubble区域

    int left_margin = this->layout()->contentsMargins().left();
    int right_margin = this->layout()->contentsMargins().right();
    int v_margin = this->layout()->contentsMargins().bottom();
    setFixedSize(pix.width()+left_margin+right_margin,pix.height()+v_margin*2);
}
