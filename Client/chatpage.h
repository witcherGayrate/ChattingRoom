#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include"chatitembase.h"
#include"userdata.h"
namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
    void SetUserInfo(std::shared_ptr<UserInfo> user_info);
    void AppendChatMsg(std::shared_ptr<TextChatData> msg);
private:
    Ui::ChatPage *ui;
    std::shared_ptr<UserInfo> _user_info;
    // QWidget interface
protected:
    //qwidget本身提供的样式不够丰富，重写paintevent满足自定义样式
    virtual void paintEvent(QPaintEvent *event) override;
private slots:
    void on_send_btn_clicked();
    void on_recv_btn_clicked();
    void slot_edit_packedmsg(ChatItemBase*);//响应来自messageedit的发送信号
signals:
    void sig_append_send_chat_msg(std::shared_ptr<TextChatData>msg);

};



#endif // CHATPAGE_H
