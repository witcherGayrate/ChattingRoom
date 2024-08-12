#ifndef CHATUSERLIST_H
#define CHATUSERLIST_H
#include<QListWidget>
#include<QWheelEvent>
#include<QEvent>
#include<QScrollBar>
#include<QDebug>

class ChatUserList:public QListWidget
{
    Q_OBJECT
public:
    ChatUserList(QWidget* parent=nullptr);
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;//给listwidget按照事件过滤器处理鼠标/键盘事件
private:
    bool _load_pending;
    // QObject interface
signals:
    void sig_loading_chat_user();

};



#endif // CHATUSERLIST_H
