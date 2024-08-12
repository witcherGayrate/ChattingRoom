#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include"chatitembase.h"
namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();

private:
    Ui::ChatPage *ui;

    // QWidget interface
protected:
    //qwidget本身提供的样式不够丰富，重写paintevent满足自定义样式
    virtual void paintEvent(QPaintEvent *event) override;
private slots:
    void on_send_btn_clicked();
    void on_recv_btn_clicked();
    void slot_edit_packedmsg(ChatItemBase*);//响应来自messageedit的发送信号
};



#endif // CHATPAGE_H
