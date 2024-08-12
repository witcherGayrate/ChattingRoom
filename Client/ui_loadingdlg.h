/********************************************************************************
** Form generated from reading UI file 'loadingdlg.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADINGDLG_H
#define UI_LOADINGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoadingDlg
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *loading_lab;

    void setupUi(QDialog *LoadingDlg)
    {
        if (LoadingDlg->objectName().isEmpty())
            LoadingDlg->setObjectName("LoadingDlg");
        LoadingDlg->resize(400, 300);
        verticalLayout = new QVBoxLayout(LoadingDlg);
        verticalLayout->setObjectName("verticalLayout");
        loading_lab = new QLabel(LoadingDlg);
        loading_lab->setObjectName("loading_lab");
        loading_lab->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(loading_lab);


        retranslateUi(LoadingDlg);

        QMetaObject::connectSlotsByName(LoadingDlg);
    } // setupUi

    void retranslateUi(QDialog *LoadingDlg)
    {
        LoadingDlg->setWindowTitle(QCoreApplication::translate("LoadingDlg", "Dialog", nullptr));
        loading_lab->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoadingDlg: public Ui_LoadingDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADINGDLG_H
