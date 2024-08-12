/********************************************************************************
** Form generated from reading UI file 'chatpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATPAGE_H
#define UI_CHATPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <chatview.h>
#include <clickedbtn.h>
#include <clickedlabel.h>
#include <messagetextedit.h>

QT_BEGIN_NAMESPACE

class Ui_ChatPage
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *chat_data_wid;
    QVBoxLayout *verticalLayout_4;
    QWidget *title_wid;
    QVBoxLayout *verticalLayout_5;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *title_lab;
    ChatView *chat_data_list;
    QWidget *tool_wid;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    ClickedLabel *emo_lab;
    QSpacerItem *horizontalSpacer_2;
    ClickedLabel *file_lab;
    QSpacerItem *horizontalSpacer_4;
    MessageTextEdit *chatEdit;
    QWidget *send_wid;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_5;
    ClickedBtn *recv_btn;
    QSpacerItem *horizontalSpacer_6;
    ClickedBtn *send_btn;
    QSpacerItem *horizontalSpacer_7;

    void setupUi(QWidget *ChatPage)
    {
        if (ChatPage->objectName().isEmpty())
            ChatPage->setObjectName("ChatPage");
        ChatPage->resize(662, 514);
        horizontalLayout = new QHBoxLayout(ChatPage);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        chat_data_wid = new QWidget(ChatPage);
        chat_data_wid->setObjectName("chat_data_wid");
        verticalLayout_4 = new QVBoxLayout(chat_data_wid);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        title_wid = new QWidget(chat_data_wid);
        title_wid->setObjectName("title_wid");
        title_wid->setMinimumSize(QSize(0, 40));
        title_wid->setMaximumSize(QSize(16777215, 40));
        verticalLayout_5 = new QVBoxLayout(title_wid);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(-1, 2, 2, 2);
        widget_2 = new QWidget(title_wid);
        widget_2->setObjectName("widget_2");
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(2, 2, 2, 2);
        title_lab = new QLabel(widget_2);
        title_lab->setObjectName("title_lab");

        horizontalLayout_3->addWidget(title_lab);


        verticalLayout_5->addWidget(widget_2);


        verticalLayout_4->addWidget(title_wid);

        chat_data_list = new ChatView(chat_data_wid);
        chat_data_list->setObjectName("chat_data_list");

        verticalLayout_4->addWidget(chat_data_list);

        tool_wid = new QWidget(chat_data_wid);
        tool_wid->setObjectName("tool_wid");
        tool_wid->setMinimumSize(QSize(0, 35));
        tool_wid->setMaximumSize(QSize(16777215, 35));
        horizontalLayout_4 = new QHBoxLayout(tool_wid);
        horizontalLayout_4->setSpacing(7);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(2, 2, 2, 2);
        horizontalSpacer_3 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        emo_lab = new ClickedLabel(tool_wid);
        emo_lab->setObjectName("emo_lab");
        emo_lab->setMinimumSize(QSize(25, 25));
        emo_lab->setMaximumSize(QSize(25, 25));

        horizontalLayout_4->addWidget(emo_lab);

        horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        file_lab = new ClickedLabel(tool_wid);
        file_lab->setObjectName("file_lab");
        file_lab->setMinimumSize(QSize(25, 25));
        file_lab->setMaximumSize(QSize(25, 25));

        horizontalLayout_4->addWidget(file_lab);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout_4->addWidget(tool_wid);

        chatEdit = new MessageTextEdit(chat_data_wid);
        chatEdit->setObjectName("chatEdit");
        chatEdit->setMinimumSize(QSize(0, 120));
        chatEdit->setMaximumSize(QSize(16777215, 120));
        chatEdit->setAcceptDrops(true);

        verticalLayout_4->addWidget(chatEdit);

        send_wid = new QWidget(chat_data_wid);
        send_wid->setObjectName("send_wid");
        send_wid->setMinimumSize(QSize(0, 40));
        send_wid->setMaximumSize(QSize(16777215, 40));
        horizontalLayout_5 = new QHBoxLayout(send_wid);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(-1, 2, -1, 2);
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        recv_btn = new ClickedBtn(send_wid);
        recv_btn->setObjectName("recv_btn");
        recv_btn->setMinimumSize(QSize(100, 30));
        recv_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout_5->addWidget(recv_btn);

        horizontalSpacer_6 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);

        send_btn = new ClickedBtn(send_wid);
        send_btn->setObjectName("send_btn");
        send_btn->setMinimumSize(QSize(100, 30));
        send_btn->setMaximumSize(QSize(100, 30));
        send_btn->setSizeIncrement(QSize(0, 0));

        horizontalLayout_5->addWidget(send_btn);

        horizontalSpacer_7 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);


        verticalLayout_4->addWidget(send_wid);


        horizontalLayout->addWidget(chat_data_wid);


        retranslateUi(ChatPage);

        QMetaObject::connectSlotsByName(ChatPage);
    } // setupUi

    void retranslateUi(QWidget *ChatPage)
    {
        ChatPage->setWindowTitle(QCoreApplication::translate("ChatPage", "Form", nullptr));
        title_lab->setText(QCoreApplication::translate("ChatPage", "\350\213\227\346\235\241\347\232\204\350\203\226\347\272\242", nullptr));
        emo_lab->setText(QString());
        file_lab->setText(QString());
        recv_btn->setText(QCoreApplication::translate("ChatPage", "\346\216\245\346\224\266", nullptr));
        send_btn->setText(QCoreApplication::translate("ChatPage", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatPage: public Ui_ChatPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATPAGE_H
