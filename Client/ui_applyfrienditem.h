/********************************************************************************
** Form generated from reading UI file 'applyfrienditem.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPLYFRIENDITEM_H
#define UI_APPLYFRIENDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <clickedbtn.h>

QT_BEGIN_NAMESPACE

class Ui_ApplyFriendItem
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *icon_lab;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QLabel *user_name_lab;
    QLabel *user_chat_lab;
    QSpacerItem *horizontalSpacer_3;
    ClickedBtn *addBtn;
    QLabel *already_add_lab;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *ApplyFriendItem)
    {
        if (ApplyFriendItem->objectName().isEmpty())
            ApplyFriendItem->setObjectName("ApplyFriendItem");
        ApplyFriendItem->resize(492, 80);
        ApplyFriendItem->setMinimumSize(QSize(0, 80));
        ApplyFriendItem->setMaximumSize(QSize(16777215, 80));
        ApplyFriendItem->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(ApplyFriendItem);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        icon_lab = new QLabel(ApplyFriendItem);
        icon_lab->setObjectName("icon_lab");
        icon_lab->setMinimumSize(QSize(45, 45));
        icon_lab->setMaximumSize(QSize(45, 45));

        horizontalLayout->addWidget(icon_lab);

        widget = new QWidget(ApplyFriendItem);
        widget->setObjectName("widget");
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(-1, 2, -1, 2);
        user_name_lab = new QLabel(widget_2);
        user_name_lab->setObjectName("user_name_lab");
        user_name_lab->setMinimumSize(QSize(100, 30));
        user_name_lab->setMaximumSize(QSize(200, 30));

        verticalLayout->addWidget(user_name_lab);

        user_chat_lab = new QLabel(widget_2);
        user_chat_lab->setObjectName("user_chat_lab");
        user_chat_lab->setMinimumSize(QSize(100, 30));
        user_chat_lab->setMaximumSize(QSize(200, 30));

        verticalLayout->addWidget(user_chat_lab);


        horizontalLayout_2->addWidget(widget_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        addBtn = new ClickedBtn(widget);
        addBtn->setObjectName("addBtn");
        addBtn->setMinimumSize(QSize(80, 30));
        addBtn->setMaximumSize(QSize(80, 30));

        horizontalLayout_2->addWidget(addBtn);

        already_add_lab = new QLabel(widget);
        already_add_lab->setObjectName("already_add_lab");
        already_add_lab->setMinimumSize(QSize(40, 0));

        horizontalLayout_2->addWidget(already_add_lab);

        horizontalSpacer_2 = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        horizontalLayout->addWidget(widget);


        retranslateUi(ApplyFriendItem);

        QMetaObject::connectSlotsByName(ApplyFriendItem);
    } // setupUi

    void retranslateUi(QWidget *ApplyFriendItem)
    {
        ApplyFriendItem->setWindowTitle(QCoreApplication::translate("ApplyFriendItem", "Form", nullptr));
        icon_lab->setText(QCoreApplication::translate("ApplyFriendItem", "TextLabel", nullptr));
        user_name_lab->setText(QCoreApplication::translate("ApplyFriendItem", "TextLabel", nullptr));
        user_chat_lab->setText(QCoreApplication::translate("ApplyFriendItem", "TextLabel", nullptr));
        addBtn->setText(QCoreApplication::translate("ApplyFriendItem", "\346\267\273\345\212\240", nullptr));
        already_add_lab->setText(QCoreApplication::translate("ApplyFriendItem", "\345\267\262\346\267\273\345\212\240", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ApplyFriendItem: public Ui_ApplyFriendItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPLYFRIENDITEM_H
