/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include "clickedlabel.h"

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QFrame *frame;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *lab_user;
    QLineEdit *lineEdit_user;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lab_password;
    QLineEdit *lineEdit_pass;
    ClickedLabel *pwd_visible;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    ClickedLabel *lab_forget;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_login;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_logup;
    QSpacerItem *horizontalSpacer_5;
    QLabel *lab_tip;
    QLabel *lab_photo;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(400, 600);
        LoginDialog->setMinimumSize(QSize(400, 600));
        LoginDialog->setMaximumSize(QSize(400, 600));
        LoginDialog->setMouseTracking(false);
        LoginDialog->setStyleSheet(QString::fromUtf8(""));
        frame = new QFrame(LoginDialog);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(20, 410, 351, 161));
        frame->setStyleSheet(QString::fromUtf8(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        lab_user = new QLabel(frame);
        lab_user->setObjectName("lab_user");
        QFont font;
        font.setFamilies({QString::fromUtf8("\346\226\271\346\255\243\346\260\264\351\273\221\347\256\200\344\275\223")});
        font.setPointSize(14);
        lab_user->setFont(font);

        horizontalLayout->addWidget(lab_user);

        lineEdit_user = new QLineEdit(frame);
        lineEdit_user->setObjectName("lineEdit_user");

        horizontalLayout->addWidget(lineEdit_user);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        lab_password = new QLabel(frame);
        lab_password->setObjectName("lab_password");
        lab_password->setMinimumSize(QSize(36, 0));
        lab_password->setFont(font);

        horizontalLayout_2->addWidget(lab_password);

        lineEdit_pass = new QLineEdit(frame);
        lineEdit_pass->setObjectName("lineEdit_pass");
        lineEdit_pass->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(lineEdit_pass);

        pwd_visible = new ClickedLabel(frame);
        pwd_visible->setObjectName("pwd_visible");
        pwd_visible->setMinimumSize(QSize(30, 0));
        pwd_visible->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_2->addWidget(pwd_visible);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 3);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer = new QSpacerItem(168, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        lab_forget = new ClickedLabel(frame);
        lab_forget->setObjectName("lab_forget");
        lab_forget->setFont(font);
        lab_forget->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(lab_forget);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(63, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 0, 1, 1);

        btn_login = new QPushButton(frame);
        btn_login->setObjectName("btn_login");
        btn_login->setFont(font);
        btn_login->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(btn_login, 3, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(63, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 3, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(63, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 4, 0, 1, 1);

        btn_logup = new QPushButton(frame);
        btn_logup->setObjectName("btn_logup");
        btn_logup->setFont(font);

        gridLayout->addWidget(btn_logup, 4, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(63, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 4, 2, 1, 1);

        lab_tip = new QLabel(LoginDialog);
        lab_tip->setObjectName("lab_tip");
        lab_tip->setGeometry(QRect(0, 380, 401, 20));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\346\226\271\346\255\243\346\260\264\351\273\221\347\256\200\344\275\223")});
        font1.setPointSize(15);
        lab_tip->setFont(font1);
        lab_tip->setAlignment(Qt::AlignCenter);
        lab_photo = new QLabel(LoginDialog);
        lab_photo->setObjectName("lab_photo");
        lab_photo->setGeometry(QRect(0, 0, 401, 341));
        lab_photo->setAlignment(Qt::AlignCenter);

        retranslateUi(LoginDialog);

        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Dialog", nullptr));
        lab_user->setText(QCoreApplication::translate("LoginDialog", "\351\202\256\347\256\261", nullptr));
        lab_password->setText(QCoreApplication::translate("LoginDialog", "\345\257\206\347\240\201", nullptr));
        pwd_visible->setText(QString());
        lab_forget->setText(QCoreApplication::translate("LoginDialog", "\345\277\230\350\256\260\345\257\206\347\240\201", nullptr));
        btn_login->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        btn_logup->setText(QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
        lab_tip->setText(QString());
        lab_photo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
