/********************************************************************************
** Form generated from reading UI file 'conuseritem.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONUSERITEM_H
#define UI_CONUSERITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConUserItem
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QLabel *icon_lab;
    QLabel *red_point;
    QLabel *user_name_lab;

    void setupUi(QWidget *ConUserItem)
    {
        if (ConUserItem->objectName().isEmpty())
            ConUserItem->setObjectName("ConUserItem");
        ConUserItem->resize(400, 70);
        ConUserItem->setMinimumSize(QSize(0, 70));
        ConUserItem->setMaximumSize(QSize(16777215, 70));
        horizontalLayout = new QHBoxLayout(ConUserItem);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(6, 2, 6, 2);
        widget = new QWidget(ConUserItem);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(60, 60));
        widget->setMaximumSize(QSize(60, 60));
        icon_lab = new QLabel(widget);
        icon_lab->setObjectName("icon_lab");
        icon_lab->setGeometry(QRect(10, 0, 45, 45));
        icon_lab->setMinimumSize(QSize(45, 45));
        icon_lab->setMaximumSize(QSize(45, 45));
        red_point = new QLabel(widget);
        red_point->setObjectName("red_point");
        red_point->setGeometry(QRect(30, 0, 30, 30));
        red_point->setMinimumSize(QSize(30, 30));
        red_point->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(widget);

        user_name_lab = new QLabel(ConUserItem);
        user_name_lab->setObjectName("user_name_lab");

        horizontalLayout->addWidget(user_name_lab);


        retranslateUi(ConUserItem);

        QMetaObject::connectSlotsByName(ConUserItem);
    } // setupUi

    void retranslateUi(QWidget *ConUserItem)
    {
        ConUserItem->setWindowTitle(QCoreApplication::translate("ConUserItem", "Form", nullptr));
        icon_lab->setText(QString());
        red_point->setText(QString());
        user_name_lab->setText(QCoreApplication::translate("ConUserItem", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConUserItem: public Ui_ConUserItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONUSERITEM_H
