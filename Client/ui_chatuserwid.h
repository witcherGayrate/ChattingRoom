/********************************************************************************
** Form generated from reading UI file 'chatuserwid.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATUSERWID_H
#define UI_CHATUSERWID_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatUserWid
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *icon_wid;
    QHBoxLayout *horizontalLayout_2;
    QLabel *icon_lab;
    QWidget *user_info_wid;
    QVBoxLayout *verticalLayout;
    QLabel *user_name_lab;
    QLabel *user_chat_lab;
    QWidget *time_wid;
    QVBoxLayout *verticalLayout_2;
    QLabel *time_lab;

    void setupUi(QWidget *ChatUserWid)
    {
        if (ChatUserWid->objectName().isEmpty())
            ChatUserWid->setObjectName("ChatUserWid");
        ChatUserWid->resize(400, 74);
        horizontalLayout = new QHBoxLayout(ChatUserWid);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(6, 2, 6, 2);
        icon_wid = new QWidget(ChatUserWid);
        icon_wid->setObjectName("icon_wid");
        icon_wid->setMinimumSize(QSize(50, 50));
        icon_wid->setMaximumSize(QSize(50, 50));
        horizontalLayout_2 = new QHBoxLayout(icon_wid);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        icon_lab = new QLabel(icon_wid);
        icon_lab->setObjectName("icon_lab");
        icon_lab->setMinimumSize(QSize(45, 45));
        icon_lab->setMaximumSize(QSize(45, 45));
        icon_lab->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(icon_lab);


        horizontalLayout->addWidget(icon_wid);

        user_info_wid = new QWidget(ChatUserWid);
        user_info_wid->setObjectName("user_info_wid");
        user_info_wid->setMinimumSize(QSize(0, 50));
        user_info_wid->setMaximumSize(QSize(16777215, 50));
        verticalLayout = new QVBoxLayout(user_info_wid);
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        user_name_lab = new QLabel(user_info_wid);
        user_name_lab->setObjectName("user_name_lab");

        verticalLayout->addWidget(user_name_lab);

        user_chat_lab = new QLabel(user_info_wid);
        user_chat_lab->setObjectName("user_chat_lab");

        verticalLayout->addWidget(user_chat_lab);


        horizontalLayout->addWidget(user_info_wid);

        time_wid = new QWidget(ChatUserWid);
        time_wid->setObjectName("time_wid");
        time_wid->setMinimumSize(QSize(50, 50));
        time_wid->setMaximumSize(QSize(50, 50));
        verticalLayout_2 = new QVBoxLayout(time_wid);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(-1, 4, -1, 7);
        time_lab = new QLabel(time_wid);
        time_lab->setObjectName("time_lab");
        time_lab->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_2->addWidget(time_lab);


        horizontalLayout->addWidget(time_wid);


        retranslateUi(ChatUserWid);

        QMetaObject::connectSlotsByName(ChatUserWid);
    } // setupUi

    void retranslateUi(QWidget *ChatUserWid)
    {
        ChatUserWid->setWindowTitle(QCoreApplication::translate("ChatUserWid", "Form", nullptr));
        icon_lab->setText(QString());
        user_name_lab->setText(QCoreApplication::translate("ChatUserWid", "redred", nullptr));
        user_chat_lab->setText(QCoreApplication::translate("ChatUserWid", "\344\273\212\345\244\251\344\270\255\345\215\210\345\220\203\344\273\200\344\271\210\345\221\200\357\274\237", nullptr));
        time_lab->setText(QCoreApplication::translate("ChatUserWid", "11:45", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatUserWid: public Ui_ChatUserWid {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATUSERWID_H
