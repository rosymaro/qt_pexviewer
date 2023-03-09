/********************************************************************************
** Form generated from reading UI file 'forminfo.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORMINFO_H
#define UI_FORMINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FormInfo
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *infoZoom;
    QLabel *pointZ;
    QLabel *infoTilt;
    QLabel *infoRot;
    QLabel *pointY;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *pointX;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QListWidget *listWidget;

    void setupUi(QDialog *FormInfo)
    {
        if (FormInfo->objectName().isEmpty())
            FormInfo->setObjectName(QString::fromUtf8("FormInfo"));
        FormInfo->resize(200, 400);
        FormInfo->setMaximumSize(QSize(200, 16777215));
        verticalLayout = new QVBoxLayout(FormInfo);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(FormInfo);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 20));
        label->setMaximumSize(QSize(16777215, 20));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        infoZoom = new QLabel(FormInfo);
        infoZoom->setObjectName(QString::fromUtf8("infoZoom"));
        sizePolicy.setHeightForWidth(infoZoom->sizePolicy().hasHeightForWidth());
        infoZoom->setSizePolicy(sizePolicy);
        infoZoom->setMinimumSize(QSize(0, 20));
        infoZoom->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(infoZoom, 3, 2, 1, 1);

        pointZ = new QLabel(FormInfo);
        pointZ->setObjectName(QString::fromUtf8("pointZ"));
        sizePolicy.setHeightForWidth(pointZ->sizePolicy().hasHeightForWidth());
        pointZ->setSizePolicy(sizePolicy);
        pointZ->setMinimumSize(QSize(0, 20));
        pointZ->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(pointZ, 1, 2, 1, 1);

        infoTilt = new QLabel(FormInfo);
        infoTilt->setObjectName(QString::fromUtf8("infoTilt"));
        sizePolicy.setHeightForWidth(infoTilt->sizePolicy().hasHeightForWidth());
        infoTilt->setSizePolicy(sizePolicy);
        infoTilt->setMinimumSize(QSize(0, 20));
        infoTilt->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(infoTilt, 3, 0, 1, 1);

        infoRot = new QLabel(FormInfo);
        infoRot->setObjectName(QString::fromUtf8("infoRot"));
        sizePolicy.setHeightForWidth(infoRot->sizePolicy().hasHeightForWidth());
        infoRot->setSizePolicy(sizePolicy);
        infoRot->setMinimumSize(QSize(0, 20));
        infoRot->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(infoRot, 3, 1, 1, 1);

        pointY = new QLabel(FormInfo);
        pointY->setObjectName(QString::fromUtf8("pointY"));
        sizePolicy.setHeightForWidth(pointY->sizePolicy().hasHeightForWidth());
        pointY->setSizePolicy(sizePolicy);
        pointY->setMinimumSize(QSize(0, 20));
        pointY->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(pointY, 1, 1, 1, 1);

        label_2 = new QLabel(FormInfo);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(0, 20));
        label_2->setMaximumSize(QSize(16777215, 20));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_4 = new QLabel(FormInfo);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(0, 20));
        label_4->setMaximumSize(QSize(16777215, 20));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_4, 0, 2, 1, 1);

        pointX = new QLabel(FormInfo);
        pointX->setObjectName(QString::fromUtf8("pointX"));
        sizePolicy.setHeightForWidth(pointX->sizePolicy().hasHeightForWidth());
        pointX->setSizePolicy(sizePolicy);
        pointX->setMinimumSize(QSize(0, 20));
        pointX->setMaximumSize(QSize(16777215, 20));
        pointX->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(pointX, 1, 0, 1, 1);

        label_10 = new QLabel(FormInfo);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy);
        label_10->setMinimumSize(QSize(0, 20));
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_10, 2, 0, 1, 1);

        label_11 = new QLabel(FormInfo);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy);
        label_11->setMinimumSize(QSize(0, 20));
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_11, 2, 1, 1, 1);

        label_12 = new QLabel(FormInfo);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy);
        label_12->setMinimumSize(QSize(0, 20));
        label_12->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_12, 2, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        listWidget = new QListWidget(FormInfo);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        verticalLayout->addWidget(listWidget);


        retranslateUi(FormInfo);

        QMetaObject::connectSlotsByName(FormInfo);
    } // setupUi

    void retranslateUi(QDialog *FormInfo)
    {
        FormInfo->setWindowTitle(QCoreApplication::translate("FormInfo", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("FormInfo", "X", nullptr));
        infoZoom->setText(QString());
        pointZ->setText(QString());
        infoTilt->setText(QString());
        infoRot->setText(QString());
        pointY->setText(QString());
        label_2->setText(QCoreApplication::translate("FormInfo", "Y", nullptr));
        label_4->setText(QCoreApplication::translate("FormInfo", "Z", nullptr));
        pointX->setText(QString());
        label_10->setText(QCoreApplication::translate("FormInfo", "Tilt", nullptr));
        label_11->setText(QCoreApplication::translate("FormInfo", "Rotation", nullptr));
        label_12->setText(QCoreApplication::translate("FormInfo", "Zoom", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FormInfo: public Ui_FormInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORMINFO_H
