#include "timerbtn.h"
#include<QMouseEvent>
TimerBtn::TimerBtn(QWidget* parent):QPushButton(parent),_counter(10)
{
    _timer = new QTimer(this);

    connect(_timer,&QTimer::timeout,[this](){
        _counter--;
        if(_counter<=0)
            {
            _timer->stop();
            _counter = 10;
            this->setText("获取");
            this->setEnabled(true);
            return;
            }
        this->setText((QString::number(_counter)+"s"));
    });

}

TimerBtn::~TimerBtn()
{
    _timer->stop();
}
void TimerBtn::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        //处理鼠标释放事件
        qDebug()<<"MyButton was released!";
        this->setEnabled(false);
        this->setText(QString::number(_counter));
        _timer->start(1000);//计时器每隔1秒触发一次信号
        emit clicked();
    }
    //调用基类的mouseReleaseEvent以确保正常的事件处理（如点击效果）
    QPushButton::mouseReleaseEvent(event);
}
