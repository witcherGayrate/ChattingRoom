#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H

#include <QFrame>
#include"global.h"
#include<QHBoxLayout>


class BubbleFrame : public QFrame
{
    Q_OBJECT
public:
    BubbleFrame(ChatRole role,QWidget* parent=nullptr);
    void setWidget(QWidget *w);//添加文本widget或者图片文件
private:
    int m_margin;
    QHBoxLayout* m_pHLayout;
    ChatRole m_role;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};



#endif // BUBBLEFRAME_H
