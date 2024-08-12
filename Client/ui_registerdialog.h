/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

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
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include "clickedlabel.h"
#include "timerbtn.h"

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QFrame *frame;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *lab_user_logup;
    QLineEdit *lineEdit_user;
    QHBoxLayout *horizontalLayout_7;
    QLabel *lab_pass_logup;
    QLineEdit *lineEdit_code;
    ClickedLabel *pass_visible;
    QPushButton *btn_cancel;
    QSpacerItem *horizontalSpacer;
    QLabel *lab_tip;
    QHBoxLayout *horizontalLayout_8;
    QLabel *lab_sure;
    QLineEdit *lineEdit_code_sure;
    ClickedLabel *confirm_visible;
    QHBoxLayout *horizontalLayout_6;
    QLabel *lab_email;
    QLineEdit *lineEdit_email;
    QPushButton *btn_sure;
    QHBoxLayout *horizontalLayout_9;
    QLabel *lab_verify;
    QLineEdit *lineEdit_verifycode;
    TimerBtn *btn_acquire;
    QWidget *page_2;
    QLabel *lab_tip_return;
    QLabel *lab_tip_click;
    QPushButton *return_btn;
    QLabel *lab_photo;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(400, 600);
        RegisterDialog->setMinimumSize(QSize(400, 600));
        RegisterDialog->setMaximumSize(QSize(400, 600));
        RegisterDialog->setStyleSheet(QString::fromUtf8(""));
        stackedWidget = new QStackedWidget(RegisterDialog);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(10, 280, 381, 311));
        page = new QWidget();
        page->setObjectName("page");
        frame = new QFrame(page);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(40, 50, 298, 233));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        lab_user_logup = new QLabel(frame);
        lab_user_logup->setObjectName("lab_user_logup");
        lab_user_logup->setMaximumSize(QSize(50, 16777215));
        QFont font;
        font.setFamilies({QString::fromUtf8("\346\226\271\346\255\243\346\260\264\351\273\221\347\256\200\344\275\223")});
        font.setPointSize(15);
        lab_user_logup->setFont(font);

        horizontalLayout_5->addWidget(lab_user_logup);

        lineEdit_user = new QLineEdit(frame);
        lineEdit_user->setObjectName("lineEdit_user");
        lineEdit_user->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_5->addWidget(lineEdit_user);


        gridLayout->addLayout(horizontalLayout_5, 1, 0, 1, 3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        lab_pass_logup = new QLabel(frame);
        lab_pass_logup->setObjectName("lab_pass_logup");
        lab_pass_logup->setMaximumSize(QSize(50, 16777215));
        lab_pass_logup->setFont(font);

        horizontalLayout_7->addWidget(lab_pass_logup);

        lineEdit_code = new QLineEdit(frame);
        lineEdit_code->setObjectName("lineEdit_code");
        lineEdit_code->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_7->addWidget(lineEdit_code);

        pass_visible = new ClickedLabel(frame);
        pass_visible->setObjectName("pass_visible");
        pass_visible->setMinimumSize(QSize(20, 20));
        pass_visible->setMaximumSize(QSize(20, 20));
        pass_visible->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_7->addWidget(pass_visible);


        gridLayout->addLayout(horizontalLayout_7, 3, 0, 1, 3);

        btn_cancel = new QPushButton(frame);
        btn_cancel->setObjectName("btn_cancel");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("\346\226\271\346\255\243\346\260\264\351\273\221\347\256\200\344\275\223")});
        font1.setPointSize(14);
        btn_cancel->setFont(font1);

        gridLayout->addWidget(btn_cancel, 6, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(106, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 6, 1, 1, 1);

        lab_tip = new QLabel(frame);
        lab_tip->setObjectName("lab_tip");
        lab_tip->setFont(font);
        lab_tip->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(lab_tip, 0, 0, 1, 3);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        lab_sure = new QLabel(frame);
        lab_sure->setObjectName("lab_sure");
        lab_sure->setMaximumSize(QSize(50, 16777215));
        lab_sure->setFont(font);

        horizontalLayout_8->addWidget(lab_sure);

        lineEdit_code_sure = new QLineEdit(frame);
        lineEdit_code_sure->setObjectName("lineEdit_code_sure");
        lineEdit_code_sure->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_8->addWidget(lineEdit_code_sure);

        confirm_visible = new ClickedLabel(frame);
        confirm_visible->setObjectName("confirm_visible");
        confirm_visible->setMinimumSize(QSize(20, 20));
        confirm_visible->setMaximumSize(QSize(20, 20));
        confirm_visible->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_8->addWidget(confirm_visible);


        gridLayout->addLayout(horizontalLayout_8, 4, 0, 1, 3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        lab_email = new QLabel(frame);
        lab_email->setObjectName("lab_email");
        lab_email->setMaximumSize(QSize(50, 16777215));
        lab_email->setFont(font);

        horizontalLayout_6->addWidget(lab_email);

        lineEdit_email = new QLineEdit(frame);
        lineEdit_email->setObjectName("lineEdit_email");
        lineEdit_email->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_6->addWidget(lineEdit_email);


        gridLayout->addLayout(horizontalLayout_6, 2, 0, 1, 3);

        btn_sure = new QPushButton(frame);
        btn_sure->setObjectName("btn_sure");
        btn_sure->setFont(font1);

        gridLayout->addWidget(btn_sure, 6, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        lab_verify = new QLabel(frame);
        lab_verify->setObjectName("lab_verify");
        lab_verify->setMaximumSize(QSize(80, 16777215));
        lab_verify->setFont(font);

        horizontalLayout_9->addWidget(lab_verify);

        lineEdit_verifycode = new QLineEdit(frame);
        lineEdit_verifycode->setObjectName("lineEdit_verifycode");
        lineEdit_verifycode->setMaximumSize(QSize(160, 16777215));

        horizontalLayout_9->addWidget(lineEdit_verifycode);

        btn_acquire = new TimerBtn(frame);
        btn_acquire->setObjectName("btn_acquire");
        btn_acquire->setFont(font1);

        horizontalLayout_9->addWidget(btn_acquire);


        gridLayout->addLayout(horizontalLayout_9, 5, 0, 1, 3);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        lab_tip_return = new QLabel(page_2);
        lab_tip_return->setObjectName("lab_tip_return");
        lab_tip_return->setGeometry(QRect(80, 70, 221, 20));
        lab_tip_return->setAlignment(Qt::AlignCenter);
        lab_tip_click = new QLabel(page_2);
        lab_tip_click->setObjectName("lab_tip_click");
        lab_tip_click->setGeometry(QRect(80, 130, 221, 20));
        lab_tip_click->setAlignment(Qt::AlignCenter);
        return_btn = new QPushButton(page_2);
        return_btn->setObjectName("return_btn");
        return_btn->setGeometry(QRect(140, 210, 80, 23));
        stackedWidget->addWidget(page_2);
        lab_photo = new QLabel(RegisterDialog);
        lab_photo->setObjectName("lab_photo");
        lab_photo->setGeometry(QRect(2, 4, 391, 301));
        lab_photo->setAlignment(Qt::AlignCenter);

        retranslateUi(RegisterDialog);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        lab_user_logup->setText(QCoreApplication::translate("RegisterDialog", "\347\224\250\346\210\267", nullptr));
        lab_pass_logup->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\347\240\201", nullptr));
        pass_visible->setText(QString());
        btn_cancel->setText(QCoreApplication::translate("RegisterDialog", "\345\217\226\346\266\210", nullptr));
        lab_tip->setText(QCoreApplication::translate("RegisterDialog", "\351\224\231\350\257\257\346\217\220\347\244\272", nullptr));
        lab_sure->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244", nullptr));
        confirm_visible->setText(QString());
        lab_email->setText(QCoreApplication::translate("RegisterDialog", "\351\202\256\347\256\261", nullptr));
        btn_sure->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244", nullptr));
        lab_verify->setText(QCoreApplication::translate("RegisterDialog", "\351\252\214\350\257\201\347\240\201", nullptr));
        btn_acquire->setText(QCoreApplication::translate("RegisterDialog", "\350\216\267\345\217\226", nullptr));
        lab_tip_return->setText(QCoreApplication::translate("RegisterDialog", "\346\263\250\345\206\214\346\210\220\345\212\237\357\274\2145s\345\220\216\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
        lab_tip_click->setText(QCoreApplication::translate("RegisterDialog", "\345\217\257\347\202\271\345\207\273\350\277\224\345\233\236\346\214\211\351\222\256\347\233\264\346\216\245\350\277\224\345\233\236\347\231\273\345\275\225\347\225\214\351\235\242", nullptr));
        return_btn->setText(QCoreApplication::translate("RegisterDialog", "\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
        lab_photo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
