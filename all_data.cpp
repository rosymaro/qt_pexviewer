#include "all_data.h"
#include <QString>
#include <QTextStream>
#include <QFileDialog>
#include <QVector>

all_data::all_data(QObject *parent) :
    QObject(parent)
{

}

all_data::~all_data()
{

}


void all_data::receiveSelectFileName(QString file_name)
{
    int row = 0;
    int column = 0;
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString getLine;
    QStringList list;
    QTextStream fileStream(&file);
    while (!fileStream.atEnd()) {
            getLine.append(fileStream.readLine());
            getLine.append(" ");
            list << (getLine.split(" ", Qt::SkipEmptyParts));
            row++;
            getLine.clear();
    }   // while End

    column = list.count()/row;

    for(int i=0 ; i<row ; i++)
    {
           QVector <QString> tempvector;
           for(int j=0 ; j<column ; j++)
           {
               int num1;
               num1 = j + column*i;
               tempvector.append(list[num1]);
           }
           inputDataVector.append(tempvector);
    }
    emit sendSplitData(row, column, inputDataVector);
    return;
}
