/********************************************************************************
** Form generated from reading UI file 'formlayer.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMLAYER_H
#define UI_FORMLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FormLayer
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;

    void setupUi(QDialog *FormLayer)
    {
        if (FormLayer->objectName().isEmpty())
            FormLayer->setObjectName(QString::fromUtf8("FormLayer"));
        FormLayer->resize(450, 600);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(FormLayer->sizePolicy().hasHeightForWidth());
        FormLayer->setSizePolicy(sizePolicy);
        FormLayer->setMinimumSize(QSize(450, 300));
        FormLayer->setMaximumSize(QSize(430, 16777215));
        verticalLayout = new QVBoxLayout(FormLayer);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(FormLayer);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setFocusPolicy(Qt::ClickFocus);
        tableWidget->setAlternatingRowColors(true);
        tableWidget->setTextElideMode(Qt::ElideMiddle);
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setVisible(false);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);


        retranslateUi(FormLayer);

        QMetaObject::connectSlotsByName(FormLayer);
    } // setupUi

    void retranslateUi(QDialog *FormLayer)
    {
        FormLayer->setWindowTitle(QCoreApplication::translate("FormLayer", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormLayer: public Ui_FormLayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMLAYER_H
