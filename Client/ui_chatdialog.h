/********************************************************************************
** Form generated from reading UI file 'chatdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATDIALOG_H
#define UI_CHATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <applyfriendpage.h>
#include <chatpage.h>
#include <chatuserlist.h>
#include <clickedbtn.h>
#include <contactuserlist.h>
#include <customlineedit.h>
#include <searchlist.h>
#include <statewidget.h>

QT_BEGIN_NAMESPACE

class Ui_ChatDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *side_bar;
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QLabel *side_head_lab;
    StateWidget *side_chat_lab;
    StateWidget *side_contact_lab;
    QSpacerItem *verticalSpacer;
    QWidget *chat_user_wid;
    QVBoxLayout *verticalLayout_2;
    QWidget *search_wid;
    QHBoxLayout *horizontalLayout_2;
    CustomLineEdit *search_edit;
    QSpacerItem *horizontalSpacer;
    ClickedBtn *add_btn;
    ChatUserList *chat_user_list;
    SearchList *search_list;
    ContactUserList *con_user_list;
    QWidget *chat_data_wid;
    QVBoxLayout *verticalLayout_4;
    QStackedWidget *stackedWidget;
    ChatPage *chat_page;
    ApplyFriendPage *friend_apply_page;

    void setupUi(QDialog *ChatDialog)
    {
        if (ChatDialog->objectName().isEmpty())
            ChatDialog->setObjectName("ChatDialog");
        ChatDialog->resize(784, 555);
        horizontalLayout = new QHBoxLayout(ChatDialog);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        side_bar = new QWidget(ChatDialog);
        side_bar->setObjectName("side_bar");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(side_bar->sizePolicy().hasHeightForWidth());
        side_bar->setSizePolicy(sizePolicy);
        side_bar->setMinimumSize(QSize(56, 0));
        side_bar->setMaximumSize(QSize(56, 16777215));
        verticalLayout = new QVBoxLayout(side_bar);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 30, -1, 0);
        widget = new QWidget(side_bar);
        widget->setObjectName("widget");
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setSpacing(30);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        side_head_lab = new QLabel(widget);
        side_head_lab->setObjectName("side_head_lab");
        side_head_lab->setMinimumSize(QSize(35, 35));
        side_head_lab->setMaximumSize(QSize(35, 35));
        side_head_lab->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(side_head_lab);

        side_chat_lab = new StateWidget(widget);
        side_chat_lab->setObjectName("side_chat_lab");
        side_chat_lab->setMinimumSize(QSize(35, 35));
        side_chat_lab->setMaximumSize(QSize(35, 35));

        verticalLayout_3->addWidget(side_chat_lab);

        side_contact_lab = new StateWidget(widget);
        side_contact_lab->setObjectName("side_contact_lab");
        side_contact_lab->setMinimumSize(QSize(35, 35));
        side_contact_lab->setMaximumSize(QSize(35, 35));

        verticalLayout_3->addWidget(side_contact_lab);


        verticalLayout->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 366, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(side_bar);

        chat_user_wid = new QWidget(ChatDialog);
        chat_user_wid->setObjectName("chat_user_wid");
        chat_user_wid->setMaximumSize(QSize(250, 16777215));
        verticalLayout_2 = new QVBoxLayout(chat_user_wid);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        search_wid = new QWidget(chat_user_wid);
        search_wid->setObjectName("search_wid");
        search_wid->setMinimumSize(QSize(0, 60));
        search_wid->setMaximumSize(QSize(16777215, 60));
        horizontalLayout_2 = new QHBoxLayout(search_wid);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        search_edit = new CustomLineEdit(search_wid);
        search_edit->setObjectName("search_edit");

        horizontalLayout_2->addWidget(search_edit);

        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        add_btn = new ClickedBtn(search_wid);
        add_btn->setObjectName("add_btn");
        add_btn->setMinimumSize(QSize(24, 24));
        add_btn->setMaximumSize(QSize(24, 24));

        horizontalLayout_2->addWidget(add_btn);


        verticalLayout_2->addWidget(search_wid);

        chat_user_list = new ChatUserList(chat_user_wid);
        chat_user_list->setObjectName("chat_user_list");

        verticalLayout_2->addWidget(chat_user_list);

        search_list = new SearchList(chat_user_wid);
        search_list->setObjectName("search_list");

        verticalLayout_2->addWidget(search_list);

        con_user_list = new ContactUserList(chat_user_wid);
        con_user_list->setObjectName("con_user_list");

        verticalLayout_2->addWidget(con_user_list);


        horizontalLayout->addWidget(chat_user_wid);

        chat_data_wid = new QWidget(ChatDialog);
        chat_data_wid->setObjectName("chat_data_wid");
        verticalLayout_4 = new QVBoxLayout(chat_data_wid);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        stackedWidget = new QStackedWidget(chat_data_wid);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setAcceptDrops(true);
        chat_page = new ChatPage();
        chat_page->setObjectName("chat_page");
        stackedWidget->addWidget(chat_page);
        friend_apply_page = new ApplyFriendPage();
        friend_apply_page->setObjectName("friend_apply_page");
        stackedWidget->addWidget(friend_apply_page);

        verticalLayout_4->addWidget(stackedWidget);


        horizontalLayout->addWidget(chat_data_wid);


        retranslateUi(ChatDialog);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ChatDialog);
    } // setupUi

    void retranslateUi(QDialog *ChatDialog)
    {
        ChatDialog->setWindowTitle(QCoreApplication::translate("ChatDialog", "Dialog", nullptr));
        side_head_lab->setText(QString());
        add_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChatDialog: public Ui_ChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATDIALOG_H
