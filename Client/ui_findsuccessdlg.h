/********************************************************************************
** Form generated from reading UI file 'findsuccessdlg.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDSUCCESSDLG_H
#define UI_FINDSUCCESSDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <clickedbtn.h>

QT_BEGIN_NAMESPACE

class Ui_FindSuccessDlg
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *head_lab;
    QLabel *name_lab;
    QSpacerItem *horizontalSpacer;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    ClickedBtn *add_friend_btn;

    void setupUi(QWidget *FindSuccessDlg)
    {
        if (FindSuccessDlg->objectName().isEmpty())
            FindSuccessDlg->setObjectName("FindSuccessDlg");
        FindSuccessDlg->resize(280, 190);
        FindSuccessDlg->setMinimumSize(QSize(280, 190));
        FindSuccessDlg->setMaximumSize(QSize(280, 190));
        verticalLayout = new QVBoxLayout(FindSuccessDlg);
        verticalLayout->setObjectName("verticalLayout");
        widget = new QWidget(FindSuccessDlg);
        widget->setObjectName("widget");
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        head_lab = new QLabel(widget);
        head_lab->setObjectName("head_lab");
        head_lab->setMinimumSize(QSize(50, 50));
        head_lab->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(head_lab);

        name_lab = new QLabel(widget);
        name_lab->setObjectName("name_lab");

        horizontalLayout->addWidget(name_lab);

        horizontalSpacer = new QSpacerItem(126, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget);

        widget_2 = new QWidget(FindSuccessDlg);
        widget_2->setObjectName("widget_2");
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        add_friend_btn = new ClickedBtn(widget_2);
        add_friend_btn->setObjectName("add_friend_btn");
        add_friend_btn->setMinimumSize(QSize(120, 35));
        add_friend_btn->setMaximumSize(QSize(120, 35));

        horizontalLayout_2->addWidget(add_friend_btn);


        verticalLayout->addWidget(widget_2);


        retranslateUi(FindSuccessDlg);

        QMetaObject::connectSlotsByName(FindSuccessDlg);
    } // setupUi

    void retranslateUi(QWidget *FindSuccessDlg)
    {
        FindSuccessDlg->setWindowTitle(QCoreApplication::translate("FindSuccessDlg", "Form", nullptr));
        head_lab->setText(QString());
        name_lab->setText(QCoreApplication::translate("FindSuccessDlg", "TextLabel", nullptr));
        add_friend_btn->setText(QCoreApplication::translate("FindSuccessDlg", "\346\267\273\345\212\240\345\210\260\351\200\232\350\256\257\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindSuccessDlg: public Ui_FindSuccessDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDSUCCESSDLG_H
