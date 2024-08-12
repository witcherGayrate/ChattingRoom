#ifndef CHATITEMBASE_H
#define CHATITEMBASE_H

#include <QWidget>
#include"global.h"
#include<QLabel>
class ChatItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ChatItemBase(ChatRole role,QWidget *parent = nullptr);

    void setUserIcon(const QPixmap pixMap);
    void setUserName(const QString name);
    void setWidget(QWidget* w);
private:
    QLabel* m_pNameLab;
    QLabel* m_pUserIconLab;
    QWidget* m_pBubble;
signals:
};

#endif // CHATITEMBASE_H
