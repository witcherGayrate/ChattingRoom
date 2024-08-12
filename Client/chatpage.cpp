#include "chatpage.h"
#include "ui_chatpage.h"
#include<QStyleOption>
#include<QPainter>
#include"chatitembase.h"
#include"textbubble.h"
#include"picturebubble.h"
ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    //初始化clickedlab的状态
    ui->emo_lab->SetState("normal","hover","press","normal","hover","press");
    ui->file_lab->SetState("normal","hover","press","normal","hover","press");
    //初始化clickedbtn的状态
    ui->recv_btn->SetState("normal","hover","press");
    ui->send_btn->SetState("normal","hover","press");

    connect(ui->chatEdit,&MessageTextEdit::sig_edit_send_packeditem,this,&ChatPage::slot_edit_packedmsg);//连接enter事件发送的信号，由messageedit进行封装
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void ChatPage::on_send_btn_clicked()
{
    auto pTextEdit = ui->chatEdit;

    ChatRole role = ChatRole::Self;
    QString userName = QStringLiteral("爱吃的胖红");
    QString userIcon = ":/icons/head_13.png";

    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    for(int i=0;i<msgList.size();++i)
    {
        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(userName);
        pChatItem->setUserIcon(QPixmap(userIcon));
        QWidget *pBubble = nullptr;
        if(type=="text")
        {
            pBubble = new TextBubble(role,msgList[i].content);
        }else if(type=="image")
        {
            pBubble = new PictureBubble(QPixmap(msgList[i].content),role);
        }else if(type == "file")
        {

        }if(pBubble != nullptr)
        {
            pChatItem->setWidget(pBubble);
            ui->chat_data_list->appendChatItem(pChatItem);
        }

    }
}


void ChatPage::on_recv_btn_clicked()
{
    auto pTextEdit = ui->chatEdit;

    ChatRole role = ChatRole::Other;
    QString userName = QStringLiteral("苗条的胖红");
    QString userIcon = ":/icons/head_13.png";

    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    for(int i=0;i<msgList.size();++i)
    {
        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserName(userName);
        pChatItem->setUserIcon(QPixmap(userIcon));
        QWidget *pBubble = nullptr;
        if(type=="text")
        {
            pBubble = new TextBubble(role,msgList[i].content);
        }else if(type=="image")
        {
            pBubble = new PictureBubble(QPixmap(msgList[i].content),role);
        }else if(type == "file")
        {

        }if(pBubble != nullptr)
        {
            pChatItem->setWidget(pBubble);
            ui->chat_data_list->appendChatItem(pChatItem);
        }

    }
}

void ChatPage::slot_edit_packedmsg(ChatItemBase *pChatItem)
{
    ui->chat_data_list->appendChatItem(pChatItem);//将封装好的item添加
}

