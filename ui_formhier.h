/********************************************************************************
** Form generated from reading UI file 'formhier.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMHIER_H
#define UI_FORMHIER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FormHier
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;

    void setupUi(QDialog *FormHier)
    {
        if (FormHier->objectName().isEmpty())
            FormHier->setObjectName(QString::fromUtf8("FormHier"));
        FormHier->resize(511, 400);
        FormHier->setMinimumSize(QSize(380, 100));
        verticalLayout = new QVBoxLayout(FormHier);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(FormHier);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setMinimumSize(QSize(260, 100));
        tableWidget->setBaseSize(QSize(260, 100));
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setGridStyle(Qt::SolidLine);
        tableWidget->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget->horizontalHeader()->setStretchLastSection(false);
        tableWidget->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(FormHier);

        QMetaObject::connectSlotsByName(FormHier);
    } // setupUi

    void retranslateUi(QDialog *FormHier)
    {
        FormHier->setWindowTitle(QCoreApplication::translate("FormHier", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormHier: public Ui_FormHier {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMHIER_H
