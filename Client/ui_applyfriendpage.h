/********************************************************************************
** Form generated from reading UI file 'applyfriendpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPLYFRIENDPAGE_H
#define UI_APPLYFRIENDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <applyfriendlist.h>

QT_BEGIN_NAMESPACE

class Ui_ApplyFriendPage
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *friend_apply_wid;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *friend_apply_lab;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    ApplyFriendList *apply_friend_list;

    void setupUi(QWidget *ApplyFriendPage)
    {
        if (ApplyFriendPage->objectName().isEmpty())
            ApplyFriendPage->setObjectName("ApplyFriendPage");
        ApplyFriendPage->resize(622, 518);
        verticalLayout = new QVBoxLayout(ApplyFriendPage);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        friend_apply_wid = new QWidget(ApplyFriendPage);
        friend_apply_wid->setObjectName("friend_apply_wid");
        horizontalLayout = new QHBoxLayout(friend_apply_wid);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        friend_apply_lab = new QLabel(friend_apply_wid);
        friend_apply_lab->setObjectName("friend_apply_lab");
        friend_apply_lab->setMinimumSize(QSize(0, 40));
        friend_apply_lab->setMaximumSize(QSize(16777215, 40));

        horizontalLayout->addWidget(friend_apply_lab);


        verticalLayout->addWidget(friend_apply_wid);

        widget_2 = new QWidget(ApplyFriendPage);
        widget_2->setObjectName("widget_2");
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        apply_friend_list = new ApplyFriendList(widget_2);
        apply_friend_list->setObjectName("apply_friend_list");

        verticalLayout_2->addWidget(apply_friend_list);


        verticalLayout->addWidget(widget_2);


        retranslateUi(ApplyFriendPage);

        QMetaObject::connectSlotsByName(ApplyFriendPage);
    } // setupUi

    void retranslateUi(QWidget *ApplyFriendPage)
    {
        ApplyFriendPage->setWindowTitle(QCoreApplication::translate("ApplyFriendPage", "Form", nullptr));
        friend_apply_lab->setText(QCoreApplication::translate("ApplyFriendPage", "\346\226\260\347\232\204\346\234\213\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ApplyFriendPage: public Ui_ApplyFriendPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPLYFRIENDPAGE_H
