/********************************************************************************
** Form generated from reading UI file 'formmap.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMMAP_H
#define UI_FORMMAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FormMap
{
public:
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;

    void setupUi(QDialog *FormMap)
    {
        if (FormMap->objectName().isEmpty())
            FormMap->setObjectName(QString::fromUtf8("FormMap"));
        FormMap->resize(200, 200);
        FormMap->setMinimumSize(QSize(200, 200));
        verticalLayout = new QVBoxLayout(FormMap);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(FormMap);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setMinimumSize(QSize(0, 0));
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout->addWidget(graphicsView);


        retranslateUi(FormMap);

        QMetaObject::connectSlotsByName(FormMap);
    } // setupUi

    void retranslateUi(QDialog *FormMap)
    {
        FormMap->setWindowTitle(QCoreApplication::translate("FormMap", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormMap: public Ui_FormMap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMMAP_H
