#ifndef FILEDB_H
#define FILEDB_H

#include <QVector>


class FileDb
{
public:
    FileDb();
    QVector<QVector<QVector<QList<float>>>> openFile(QString fileNameInfo);
private:
    QVector<QVector<QVector<QList<float>>>> mapFile;
};

#endif // FILEDB_H
