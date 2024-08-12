#include "customlineedit.h"


CustomLineEdit::CustomLineEdit(QWidget *parent):QLineEdit(parent),_max_len(300)
{
    connect(this,&QLineEdit::textChanged,this,&CustomLineEdit::limitTextLength);
}

void CustomLineEdit::SetMaxLength(int maxLen)
{
    _max_len = maxLen;
}

void CustomLineEdit::focusOutEvent(QFocusEvent *event)
{
    //交给基类继续处理event
    QLineEdit::focusOutEvent(event);
    //失去焦点时发送信号
    emit sig_focus_out();
}

void CustomLineEdit::limitTextLength(QString text)
{
    if(_max_len<=0)
    {
        return;
    }
    QByteArray bytes = text.toUtf8();//输入可能是汉字或者字母
    if(bytes.size()>_max_len)
    {
        bytes = bytes.left(_max_len);//截取允许的最大长度字节流
        this->setText(QString::fromUtf8(bytes));
    }
}
