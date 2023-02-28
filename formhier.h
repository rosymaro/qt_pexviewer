#ifndef FORMHIER_H
#define FORMHIER_H

#include <QDialog>
//#include "all_data.h"
//#include "mainwindow.h"

namespace Ui {
class FormHier;
}

class FormHier : public QDialog
{
    Q_OBJECT

public slots:
    void ReceiveSplitData(int row, int column, const QVector <QVector <QString>> &inputDataVector);
//    void ReceiveSplitData(QStringList list, int row, int column);

public:
    FormHier(QWidget *parent = nullptr);
    ~FormHier();
    Ui::FormHier *ui;

public:
    //MyDataSet *dataset = nullptr;

public:
    //void setMyDataSet(MyDataSet * dataset_){ this->dataset = dataset_;}
    //void testMyData();

};

#endif // FORMHIER_H
