#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QWidget>
#include<QVBoxLayout>
#include<QScrollArea>
class ChatView:public QWidget
{
    Q_OBJECT
public:
    ChatView(QWidget* parent=nullptr);


    void appendChatItem(QWidget* item);//在尾部插入item
    void prependChatItem(QWidget* item);//在头部插入
    void insertChatItem(QWidget* before,QWidget* item);
    void removeAllItem();//移除所有聊天条目
private:
    void initStyleSheet();
    bool _is_appended;
    QVBoxLayout* _m_VLayout;
    QScrollArea* _m_ScrollArea;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    // QWidget interface
    virtual void paintEvent(QPaintEvent *event) override;
private slots:
    void onVScrollBarMoved(int min,int max);
};



#endif // CHATVIEW_H
