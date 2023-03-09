/********************************************************************************
** Form generated from reading UI file 'formtop.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMTOP_H
#define UI_FORMTOP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FormTop
{
public:
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;

    void setupUi(QDialog *FormTop)
    {
        if (FormTop->objectName().isEmpty())
            FormTop->setObjectName(QString::fromUtf8("FormTop"));
        FormTop->resize(200, 200);
        FormTop->setMinimumSize(QSize(200, 200));
        verticalLayout = new QVBoxLayout(FormTop);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(FormTop);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);


        retranslateUi(FormTop);

        QMetaObject::connectSlotsByName(FormTop);
    } // setupUi

    void retranslateUi(QDialog *FormTop)
    {
        FormTop->setWindowTitle(QCoreApplication::translate("FormTop", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormTop: public Ui_FormTop {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMTOP_H
