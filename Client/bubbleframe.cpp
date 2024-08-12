#include "bubbleframe.h"
#include<QPainter>
#define WIDTH_TRIANGLE 8 //等腰三角形的边长

BubbleFrame::BubbleFrame(ChatRole role, QWidget *parent):QFrame(parent),
 m_margin(3),m_role(role)
{

    m_pHLayout = new QHBoxLayout(this);
    if(role==ChatRole::Self)
    {
        m_pHLayout->setContentsMargins(m_margin,m_margin,m_margin+WIDTH_TRIANGLE,m_margin);
    }else
    {
        m_pHLayout->setContentsMargins(m_margin+WIDTH_TRIANGLE,m_margin,m_margin,m_margin);
    }
    this->setLayout(m_pHLayout);
}

void BubbleFrame::setWidget(QWidget *w)
{
    if(m_pHLayout->count()>0)
    {
        return;
    }else
    {
        m_pHLayout->addWidget(w);
    }
}


void BubbleFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);//绘制的图形无边框

    if(m_role==ChatRole::Self)
    {
        //绘制矩形
        QColor bk_color(158,234,106);
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(0,0,this->width()-WIDTH_TRIANGLE,this->height());
        painter.drawRoundedRect(bk_rect,5,5);//绘制圆角矩形

        //绘制三角星
        QPointF points[3] = {
            QPointF(bk_rect.x()+bk_rect.width(),12),
            QPointF(bk_rect.x()+bk_rect.width(),12+WIDTH_TRIANGLE+2),
            QPointF(bk_rect.x()+bk_rect.width()+WIDTH_TRIANGLE,10+WIDTH_TRIANGLE-WIDTH_TRIANGLE/2),
        };
        painter.drawPolygon(points,3);
    }else
    {
        //画气泡
        QColor bk_color(Qt::white);
        painter.setBrush(QBrush(bk_color));
        QRect bk_rect = QRect(WIDTH_TRIANGLE, 0, this->width()-WIDTH_TRIANGLE, this->height());
        painter.drawRoundedRect(bk_rect,5,5);
        //画小三角
        QPointF points[3] = {
            QPointF(bk_rect.x(), 12),
            QPointF(bk_rect.x(), 10+WIDTH_TRIANGLE +2),
            QPointF(bk_rect.x()-WIDTH_TRIANGLE, 10+WIDTH_TRIANGLE-WIDTH_TRIANGLE/2),
        };
        painter.drawPolygon(points, 3);
    }
    //基类继续
    return QFrame::paintEvent(event);

}
