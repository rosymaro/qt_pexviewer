#include "filedb.h"
#include <QFile>
#include <QVector>
#include <QTextStream>
#include <QDebug>

FileDb::FileDb()
{

}

QVector<QVector<QVector<QList<float>>>> FileDb::openFile(QString fileNameInfo)
{
    qDebug() << "fileDb.h : " <<fileNameInfo;
    QFile file(fileNameInfo);

    float minX = 0, minY = 0, maxX = 0, maxY =0;
    QVector<QList<float>> strVector;

    if (file.open(QIODevice::ReadOnly) | (QIODevice::Text))
    {
        QTextStream openFile(&file);
        while(!openFile.atEnd())
        {
            auto dataString = openFile.readLine().split(",");
            QList<float> data;
            for (auto &tempdata : dataString)
            {
                 data.append(tempdata.toFloat());
            }
            strVector.append(data);
            qDebug()<< "strVector : " << data;
            // mapFile 개념 설정 필요
            if(data.size() == 8)
            {
                if(minX > data[2]){minX = data[2];}
                if(maxX < data[4]){maxX = data[4];}
                if(minY > data[3]){minY = data[3];}
                if(maxY < data[5]){maxY = data[5];}
            }
        }
        file.close();
    }
    else
    {
        QVector<QVector<QVector<QList<float>>>> mapFile{{{}}};
        return mapFile;
    }
    int n = (int(maxX*1000)-int(minX*1000))/10 +1;
    int m = (int(maxY*1000)-int(minY*1000))/10 +1;
    qDebug() << "int n / int m : " << n << " , " <<m;

    QVector<QVector<QVector<QList<float>>>> mapFile(n, QVector<QVector<QList<float>>>(m, {{}}));
    mapFile[0][0][0].append({0, 0, minX, minY, maxX, maxY, 0, 0});
    //Layer , unknown, x1, y1, x2, y2, z, thk


    for (auto &data : strVector)
    {
        if(data.size() == 8)
        {
            qDebug() << " " << data[2] << " "<< int(data[2]*100) << " "<< data[3] << " "<< int(data[3]*100);
            int in_n = 0, in_m = 0;
            if(data[2]<0)
            {in_n = n + int(data[2]*100)-1;}
            // -0.1 이 int 가 되면 0 이 됨 이때 가장 뒤에 들어가야 되므로
            else
            {in_n = int(data[2]*100);}
            if(data[3]<0)
            {in_m = m + int(data[3]*100)-1;}
            else
            {in_m = int(data[3]*100);}
            mapFile[in_n][in_m].append(data);
        }
    }
    qDebug() << "fileDb mapFile1 : " << &mapFile;

    return mapFile;

}
