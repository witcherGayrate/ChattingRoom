#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QPushButton>
#include <QTimer> //计时器
class TimerBtn : public QPushButton
{
    Q_OBJECT
public:
    TimerBtn(QWidget* parent = nullptr);
    ~TimerBtn();

    virtual void mouseReleaseEvent(QMouseEvent *event) override;


    // QWidget interface
private:
    QTimer *_timer;
    int _counter;

};



#endif // TIMERBTN_H
