/********************************************************************************
** Form generated from reading UI file 'resetdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESETDIALOG_H
#define UI_RESETDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include "clickedlabel.h"
#include "timerbtn.h"

QT_BEGIN_NAMESPACE

class Ui_ResetDialog
{
public:
    QFrame *frame;
    QGridLayout *gridLayout;
    QLabel *lab_user;
    QLineEdit *user_edit;
    QLabel *lab_email;
    QLineEdit *email_edit;
    QLabel *lab_verifycode;
    QLineEdit *verifycode_edit;
    TimerBtn *btn_verifycode;
    QLabel *lab_pwd;
    QLineEdit *newpwd_edit;
    ClickedLabel *newpwd_visible;
    QPushButton *btn_sure;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_return;
    QLabel *lab_error_tip;
    QLabel *lab_photo;

    void setupUi(QDialog *ResetDialog)
    {
        if (ResetDialog->objectName().isEmpty())
            ResetDialog->setObjectName("ResetDialog");
        ResetDialog->resize(400, 600);
        ResetDialog->setMinimumSize(QSize(400, 600));
        ResetDialog->setMaximumSize(QSize(400, 600));
        frame = new QFrame(ResetDialog);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(30, 360, 331, 221));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName("gridLayout");
        lab_user = new QLabel(frame);
        lab_user->setObjectName("lab_user");
        QFont font;
        font.setFamilies({QString::fromUtf8("\346\226\271\346\255\243\346\260\264\351\273\221\347\256\200\344\275\223")});
        font.setPointSize(14);
        lab_user->setFont(font);

        gridLayout->addWidget(lab_user, 0, 0, 1, 1);

        user_edit = new QLineEdit(frame);
        user_edit->setObjectName("user_edit");

        gridLayout->addWidget(user_edit, 0, 1, 1, 4);

        lab_email = new QLabel(frame);
        lab_email->setObjectName("lab_email");
        lab_email->setFont(font);

        gridLayout->addWidget(lab_email, 1, 0, 1, 1);

        email_edit = new QLineEdit(frame);
        email_edit->setObjectName("email_edit");

        gridLayout->addWidget(email_edit, 1, 1, 1, 4);

        lab_verifycode = new QLabel(frame);
        lab_verifycode->setObjectName("lab_verifycode");
        lab_verifycode->setFont(font);

        gridLayout->addWidget(lab_verifycode, 2, 0, 1, 1);

        verifycode_edit = new QLineEdit(frame);
        verifycode_edit->setObjectName("verifycode_edit");

        gridLayout->addWidget(verifycode_edit, 2, 1, 1, 2);

        btn_verifycode = new TimerBtn(frame);
        btn_verifycode->setObjectName("btn_verifycode");
        btn_verifycode->setMaximumSize(QSize(80, 16777215));
        btn_verifycode->setFont(font);

        gridLayout->addWidget(btn_verifycode, 2, 3, 1, 2);

        lab_pwd = new QLabel(frame);
        lab_pwd->setObjectName("lab_pwd");
        lab_pwd->setFont(font);

        gridLayout->addWidget(lab_pwd, 3, 0, 1, 1);

        newpwd_edit = new QLineEdit(frame);
        newpwd_edit->setObjectName("newpwd_edit");

        gridLayout->addWidget(newpwd_edit, 3, 1, 1, 3);

        newpwd_visible = new ClickedLabel(frame);
        newpwd_visible->setObjectName("newpwd_visible");
        newpwd_visible->setMaximumSize(QSize(50, 16777215));
        newpwd_visible->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(newpwd_visible, 3, 4, 1, 1);

        btn_sure = new QPushButton(frame);
        btn_sure->setObjectName("btn_sure");
        btn_sure->setFont(font);

        gridLayout->addWidget(btn_sure, 4, 0, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(122, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 4, 2, 1, 1);

        btn_return = new QPushButton(frame);
        btn_return->setObjectName("btn_return");
        btn_return->setFont(font);

        gridLayout->addWidget(btn_return, 4, 3, 1, 2);

        lab_error_tip = new QLabel(ResetDialog);
        lab_error_tip->setObjectName("lab_error_tip");
        lab_error_tip->setGeometry(QRect(0, 330, 401, 20));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\346\226\271\346\255\243\346\260\264\351\273\221\347\256\200\344\275\223")});
        font1.setPointSize(16);
        lab_error_tip->setFont(font1);
        lab_error_tip->setAlignment(Qt::AlignCenter);
        lab_photo = new QLabel(ResetDialog);
        lab_photo->setObjectName("lab_photo");
        lab_photo->setGeometry(QRect(2, 4, 391, 311));
        lab_photo->setAlignment(Qt::AlignCenter);

        retranslateUi(ResetDialog);

        QMetaObject::connectSlotsByName(ResetDialog);
    } // setupUi

    void retranslateUi(QDialog *ResetDialog)
    {
        ResetDialog->setWindowTitle(QCoreApplication::translate("ResetDialog", "Dialog", nullptr));
        lab_user->setText(QCoreApplication::translate("ResetDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        lab_email->setText(QCoreApplication::translate("ResetDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        lab_verifycode->setText(QCoreApplication::translate("ResetDialog", "\351\252\214\350\257\201\347\240\201\357\274\232", nullptr));
        btn_verifycode->setText(QCoreApplication::translate("ResetDialog", "\350\216\267\345\217\226", nullptr));
        lab_pwd->setText(QCoreApplication::translate("ResetDialog", "\346\226\260\345\257\206\347\240\201\357\274\232", nullptr));
        newpwd_visible->setText(QString());
        btn_sure->setText(QCoreApplication::translate("ResetDialog", "\347\241\256\350\256\244", nullptr));
        btn_return->setText(QCoreApplication::translate("ResetDialog", "\350\277\224\345\233\236", nullptr));
        lab_error_tip->setText(QCoreApplication::translate("ResetDialog", "\351\224\231\350\257\257\346\217\220\347\244\272", nullptr));
        lab_photo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ResetDialog: public Ui_ResetDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESETDIALOG_H
