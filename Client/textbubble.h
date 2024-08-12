#ifndef TEXTBUBBLE_H
#define TEXTBUBBLE_H
#include"bubbleframe.h"
#include<QTextEdit>
class TextBubble :public BubbleFrame
{
    Q_OBJECT
public:
    TextBubble(ChatRole role,const QString &text,QWidget* parent=nullptr);

    void setPlainText(const QString &text);
    void initStyleSheet();
    void adjustTextHeight();

    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private:
    ChatRole m_role;
    QTextEdit* m_pTextEdit;

    // QObject interface
public:

};


#endif // TEXTBUBBLE_H
