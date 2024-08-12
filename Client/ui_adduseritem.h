/********************************************************************************
** Form generated from reading UI file 'adduseritem.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDUSERITEM_H
#define UI_ADDUSERITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddUserItem
{
public:
    QGridLayout *gridLayout;
    QLabel *add_tip;
    QLabel *message_tip;
    QLabel *right_tip;

    void setupUi(QWidget *AddUserItem)
    {
        if (AddUserItem->objectName().isEmpty())
            AddUserItem->setObjectName("AddUserItem");
        AddUserItem->resize(250, 70);
        AddUserItem->setMinimumSize(QSize(250, 70));
        AddUserItem->setMaximumSize(QSize(250, 70));
        gridLayout = new QGridLayout(AddUserItem);
        gridLayout->setObjectName("gridLayout");
        add_tip = new QLabel(AddUserItem);
        add_tip->setObjectName("add_tip");
        add_tip->setMinimumSize(QSize(40, 40));
        add_tip->setMaximumSize(QSize(40, 40));

        gridLayout->addWidget(add_tip, 0, 0, 1, 1);

        message_tip = new QLabel(AddUserItem);
        message_tip->setObjectName("message_tip");
        message_tip->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(message_tip, 0, 1, 1, 1);

        right_tip = new QLabel(AddUserItem);
        right_tip->setObjectName("right_tip");
        right_tip->setMinimumSize(QSize(30, 30));
        right_tip->setMaximumSize(QSize(30, 30));

        gridLayout->addWidget(right_tip, 0, 2, 1, 1);


        retranslateUi(AddUserItem);

        QMetaObject::connectSlotsByName(AddUserItem);
    } // setupUi

    void retranslateUi(QWidget *AddUserItem)
    {
        AddUserItem->setWindowTitle(QCoreApplication::translate("AddUserItem", "Form", nullptr));
        add_tip->setText(QString());
        message_tip->setText(QCoreApplication::translate("AddUserItem", "\346\237\245\346\211\276uid/name", nullptr));
        right_tip->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AddUserItem: public Ui_AddUserItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDUSERITEM_H
