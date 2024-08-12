/********************************************************************************
** Form generated from reading UI file 'applyfriend.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPLYFRIEND_H
#define UI_APPLYFRIEND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <clickedbtn.h>
#include <clickedoncelabel.h>
#include <customlineedit.h>

QT_BEGIN_NAMESPACE

class Ui_ApplyFriend
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollcontent;
    QVBoxLayout *verticalLayout_2;
    QLabel *apply_lab;
    QWidget *apply_wid;
    QVBoxLayout *verticalLayout_3;
    QFrame *line;
    QLabel *label;
    CustomLineEdit *name_ed;
    QLabel *label_2;
    CustomLineEdit *back_ed;
    QLabel *label_3;
    QWidget *lab_group_wid;
    QVBoxLayout *verticalLayout_4;
    QWidget *grid_widget;
    CustomLineEdit *lab_ed;
    QWidget *input_tip_wid;
    QVBoxLayout *verticalLayout_5;
    ClickedOnceLabel *tip_lab;
    QWidget *Widget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *lab_list;
    QWidget *more_lab_wid;
    QVBoxLayout *verticalLayout_6;
    QSpacerItem *verticalSpacer_3;
    ClickedOnceLabel *more_lab;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer;
    QWidget *apply_sure_wid;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    ClickedBtn *sure_btn;
    QSpacerItem *horizontalSpacer_2;
    ClickedBtn *cancel_btn;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *ApplyFriend)
    {
        if (ApplyFriend->objectName().isEmpty())
            ApplyFriend->setObjectName("ApplyFriend");
        ApplyFriend->resize(321, 621);
        ApplyFriend->setMinimumSize(QSize(321, 621));
        ApplyFriend->setMaximumSize(QSize(321, 621));
        verticalLayout = new QVBoxLayout(ApplyFriend);
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(ApplyFriend);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setMinimumSize(QSize(0, 500));
        scrollArea->setWidgetResizable(true);
        scrollcontent = new QWidget();
        scrollcontent->setObjectName("scrollcontent");
        scrollcontent->setGeometry(QRect(0, 0, 319, 498));
        verticalLayout_2 = new QVBoxLayout(scrollcontent);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, -1, 0, -1);
        apply_lab = new QLabel(scrollcontent);
        apply_lab->setObjectName("apply_lab");
        apply_lab->setMinimumSize(QSize(0, 25));
        apply_lab->setMaximumSize(QSize(16777215, 25));
        apply_lab->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(apply_lab);

        apply_wid = new QWidget(scrollcontent);
        apply_wid->setObjectName("apply_wid");
        verticalLayout_3 = new QVBoxLayout(apply_wid);
        verticalLayout_3->setSpacing(7);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        line = new QFrame(apply_wid);
        line->setObjectName("line");
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        label = new QLabel(apply_wid);
        label->setObjectName("label");
        label->setMinimumSize(QSize(0, 25));
        label->setMaximumSize(QSize(16777215, 25));

        verticalLayout_3->addWidget(label);

        name_ed = new CustomLineEdit(apply_wid);
        name_ed->setObjectName("name_ed");
        name_ed->setMinimumSize(QSize(120, 35));
        name_ed->setMaximumSize(QSize(16777215, 35));

        verticalLayout_3->addWidget(name_ed);

        label_2 = new QLabel(apply_wid);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(0, 25));
        label_2->setMaximumSize(QSize(16777215, 25));

        verticalLayout_3->addWidget(label_2);

        back_ed = new CustomLineEdit(apply_wid);
        back_ed->setObjectName("back_ed");
        back_ed->setMinimumSize(QSize(120, 35));
        back_ed->setMaximumSize(QSize(16777215, 35));

        verticalLayout_3->addWidget(back_ed);

        label_3 = new QLabel(apply_wid);
        label_3->setObjectName("label_3");
        label_3->setMinimumSize(QSize(0, 25));
        label_3->setMaximumSize(QSize(16777215, 25));

        verticalLayout_3->addWidget(label_3);

        lab_group_wid = new QWidget(apply_wid);
        lab_group_wid->setObjectName("lab_group_wid");
        verticalLayout_4 = new QVBoxLayout(lab_group_wid);
        verticalLayout_4->setSpacing(7);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        grid_widget = new QWidget(lab_group_wid);
        grid_widget->setObjectName("grid_widget");
        grid_widget->setMinimumSize(QSize(0, 40));
        grid_widget->setMaximumSize(QSize(16777215, 40));
        lab_ed = new CustomLineEdit(grid_widget);
        lab_ed->setObjectName("lab_ed");
        lab_ed->setGeometry(QRect(2, 2, 120, 35));
        lab_ed->setMinimumSize(QSize(80, 35));
        lab_ed->setMaximumSize(QSize(16777215, 35));

        verticalLayout_4->addWidget(grid_widget);

        input_tip_wid = new QWidget(lab_group_wid);
        input_tip_wid->setObjectName("input_tip_wid");
        input_tip_wid->setMinimumSize(QSize(0, 35));
        input_tip_wid->setMaximumSize(QSize(16777215, 35));
        verticalLayout_5 = new QVBoxLayout(input_tip_wid);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        tip_lab = new ClickedOnceLabel(input_tip_wid);
        tip_lab->setObjectName("tip_lab");
        tip_lab->setMinimumSize(QSize(0, 0));
        QFont font;
        font.setPointSize(11);
        tip_lab->setFont(font);

        verticalLayout_5->addWidget(tip_lab);


        verticalLayout_4->addWidget(input_tip_wid);


        verticalLayout_3->addWidget(lab_group_wid);


        verticalLayout_2->addWidget(apply_wid);

        Widget = new QWidget(scrollcontent);
        Widget->setObjectName("Widget");
        Widget->setMinimumSize(QSize(0, 60));
        horizontalLayout_2 = new QHBoxLayout(Widget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        lab_list = new QWidget(Widget);
        lab_list->setObjectName("lab_list");
        lab_list->setMinimumSize(QSize(287, 60));
        lab_list->setMaximumSize(QSize(287, 16777215));

        horizontalLayout_2->addWidget(lab_list);

        more_lab_wid = new QWidget(Widget);
        more_lab_wid->setObjectName("more_lab_wid");
        more_lab_wid->setMinimumSize(QSize(30, 0));
        more_lab_wid->setMaximumSize(QSize(30, 16777215));
        verticalLayout_6 = new QVBoxLayout(more_lab_wid);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_3);

        more_lab = new ClickedOnceLabel(more_lab_wid);
        more_lab->setObjectName("more_lab");
        more_lab->setMinimumSize(QSize(25, 25));
        more_lab->setMaximumSize(QSize(25, 25));
        more_lab->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_6->addWidget(more_lab);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_4);


        horizontalLayout_2->addWidget(more_lab_wid);


        verticalLayout_2->addWidget(Widget);

        scrollArea->setWidget(scrollcontent);

        verticalLayout->addWidget(scrollArea);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        apply_sure_wid = new QWidget(ApplyFriend);
        apply_sure_wid->setObjectName("apply_sure_wid");
        apply_sure_wid->setMinimumSize(QSize(0, 50));
        horizontalLayout = new QHBoxLayout(apply_sure_wid);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(37, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        sure_btn = new ClickedBtn(apply_sure_wid);
        sure_btn->setObjectName("sure_btn");
        sure_btn->setMinimumSize(QSize(100, 30));

        horizontalLayout->addWidget(sure_btn);

        horizontalSpacer_2 = new QSpacerItem(38, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        cancel_btn = new ClickedBtn(apply_sure_wid);
        cancel_btn->setObjectName("cancel_btn");
        cancel_btn->setMinimumSize(QSize(100, 30));

        horizontalLayout->addWidget(cancel_btn);

        horizontalSpacer_3 = new QSpacerItem(37, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout->addWidget(apply_sure_wid);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(ApplyFriend);

        QMetaObject::connectSlotsByName(ApplyFriend);
    } // setupUi

    void retranslateUi(QDialog *ApplyFriend)
    {
        ApplyFriend->setWindowTitle(QCoreApplication::translate("ApplyFriend", "Dialog", nullptr));
        apply_lab->setText(QCoreApplication::translate("ApplyFriend", "\347\224\263\350\257\267\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        label->setText(QCoreApplication::translate("ApplyFriend", "\345\217\221\351\200\201\346\267\273\345\212\240\346\234\213\345\217\213\347\224\263\350\257\267\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("ApplyFriend", "\345\244\207\346\263\250\345\220\215\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("ApplyFriend", "\346\240\207\347\255\276", nullptr));
        tip_lab->setText(QCoreApplication::translate("ApplyFriend", "TextLabel", nullptr));
        more_lab->setText(QString());
        sure_btn->setText(QCoreApplication::translate("ApplyFriend", "\347\241\256\350\256\244", nullptr));
        cancel_btn->setText(QCoreApplication::translate("ApplyFriend", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ApplyFriend: public Ui_ApplyFriend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPLYFRIEND_H
