#include "formhier.h"
#include "ui_formhier.h"
#include "mainwindow.h"
#include <QTableWidget>
#include <QDebug>


FormHier::FormHier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormHier)
{
    ui->setupUi(this);
    // QTableWidget defalut 크기 설정
    QStringList defalutTableHeader;
    defalutTableHeader << "LayerName" << "LayerNum" << "LayerType" << "Red" << "Green" << "Blue";

    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,80);
    ui->tableWidget->setColumnWidth(3,50);
    ui->tableWidget->setColumnWidth(4,50);
    ui->tableWidget->setColumnWidth(5,50);
    ui->tableWidget->setHorizontalHeaderLabels(defalutTableHeader);

    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section {background-color:#404040;color:#FFFFFF;}");

//    tempcolor = ui->tableWidget->setBackgroundRole();
//    ui->tableWidget->setHorizontalHeaderItem(0,QColor(128,128,128));

}

FormHier::~FormHier()
{
    delete ui;
}

//void FormHier::testMyData(){
//    this->dataset->split_datas[0][0] = QString("No, I'm not fool!!!");
//}

//void FormHier::ReceiveSplitData(QStringList list, int row, int column,  QVector <QVector <QString>> &inputDataVector)
void FormHier::ReceiveSplitData(int row, int column, const QVector <QVector <QString>> &inputDataVector)
{
//    qDebug() << "xxxxxxxxxxxxxxxxxxxxxxxxx";
//    qDebug() << "row" << row;
//    qDebug() << "column" << column;
//    qDebug() << "1st" << inputDataVector.value(0);
//    qDebug() << "2nd" << inputDataVector.value(1);
//    qDebug() << "3rd" << inputDataVector.value(2);
//    qDebug() << "1st_1st" << inputDataVector.value(0).value(0);

// QTableWidget 에서 QStringList 로만 받기때문에 자료형변환
    QStringList vectorTOqstringlist;
    QStringList vectorTOqstringlistHoriLabels;

// QTableWidget 크기 설정
    ui->tableWidget->setColumnCount(column);
    ui->tableWidget->setRowCount(row);

// Table Header Font size/bold change
    QFont font = ui->tableWidget->horizontalHeader()->font();
    font.setBold(true);
    font.setPointSize(10);
    ui->tableWidget->horizontalHeader()->setFont(font);

// Table 채우기
    for (int i=0; i<row ; i++)
    {
        for (int j=0; j<column ; j++)
        {
            vectorTOqstringlist << inputDataVector.value(i+1).value(j);
            vectorTOqstringlistHoriLabels << inputDataVector.value(0).value(j);
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(vectorTOqstringlist[i*column+j]));
            ui->tableWidget->setHorizontalHeaderLabels(vectorTOqstringlistHoriLabels);
        }

    }
//    ui->tableWidget->set;
//    ui->tableWidget->setColumnWidth(0,80);
//    ui->tableWidget->setColumnWidth(1,80);
//    ui->tableWidget->setColumnWidth(2,80);
//    ui->tableWidget->setColumnWidth(3,50);
//    ui->tableWidget->setColumnWidth(4,50);
//    ui->tableWidget->setColumnWidth(5,50);



}
