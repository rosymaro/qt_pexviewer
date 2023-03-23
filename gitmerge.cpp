#include "gitmerge.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

//git test

GitMerge::GitMerge()
{
    split1 = hash;
    split2 = hash;
    split1.append(file);
    split2.append(code);

    QString totalMerge;
    QString preMergeFile;
    QString updateMergeFile;
    this->mergeFiles(totalMerge);
    this->openPreMergeFile(preMergeFile);
    this->openUpdateMergeFile(updateMergeFile);
    this->writeMergeFile("./gitMerge.txt", totalMerge);
    //    if (compareFiles(preMergeFile, updateMergeFile))
    //    {
    //        if(compareFiles(preMergeFile, totalMerge))
    //        {
    //            qDebug() << "gitMerge file and code files are not changed " ;
    //            return;
    //        }
    //        else
    //        {
    //            this->writeMergeFile("./gitMerge.txt", totalMerge);
    ////            this->writeMergeFile("./gitMerge.back", totalMerge);
    //        }
    //    }
    //    else
    //    {
    //        this->writeMergeFile("./gitMerge.back", updateMergeFile);
    //        this->makeCodeFiles(totalMerge, updateMergeFile);
    //    }
}

void GitMerge::mergeFiles(QString &totalMerge)
{
    QFile file("./IInterface.pro");


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading ";
        return;
    }

    QTextStream in(&file);
    QString merge = in.readAll();
    QStringList mergeList = merge.split("\n");
    qDebug()<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    for (auto &itemMerge : mergeList)
    {

        itemMerge = itemMerge.remove("\\");
        itemMerge = itemMerge.trimmed();
        //ì£¼ìì²ë¦¬´ì¼ ë¤.
        if (itemMerge.contains(".cpp", Qt::CaseInsensitive) || itemMerge.contains(".h", Qt::CaseInsensitive) || itemMerge.contains(".ui", Qt::CaseInsensitive))
        {
            QFile codeFile(itemMerge.prepend("./"));
            if(!codeFile.open(QFile::ReadOnly | QFile::Text))
            {
                qDebug() << " Could not open the code file for reading " ;
                return;
            }
            QTextStream codeIn(&codeFile);
            QString codeText = codeIn.readAll();

            totalMerge.append(split1);
            totalMerge.append(itemMerge);
            totalMerge.append(split2);
            totalMerge.append(codeText);
            codeFile.close();

        }

    }
    totalMerge.append(split1);
    totalMerge.append("./IInterface.pro");
    totalMerge.append(split2);
    totalMerge.append(merge);

    file.close();
}

void GitMerge::openPreMergeFile(QString &preMergeFile)
{
    QString fileName = "./gitMerge.back";
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading ";
        preMergeFile = "";
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            return;
        }
        QTextStream out(&file);
        out << "";
        file.close();
        return;
    }
    QTextStream in(&file);
    preMergeFile = in.readAll();
    file.close();
}
void GitMerge::openUpdateMergeFile(QString &updateMergeFile)
{
    QString fileName = "./gitMerge.txt";
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading ";
        updateMergeFile = "";
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            return;
        }
        QTextStream out(&file);
        out << "";
        file.close();
        return;
    }
    QTextStream in(&file);
    updateMergeFile = in.readAll();
    file.close();
}

bool GitMerge::compareFiles(QString &A, QString &B)
{
    if (A==B)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void GitMerge::writeMergeFile(QString fileName, QString &originFile)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for writing ";
        return;
    }
    QTextStream out(&file);
    out << originFile;
    file.flush();
    file.close();

}

void GitMerge::makeCodeFiles(QString &codeFile, QString &gitMergeFile)
{
    QStringList codeList = codeFile.split(split1);
    QStringList fileList = gitMergeFile.split(split1);
    QStringList codeListDivide;
    QStringList fileListDivide;
    for (int i = 1; i < fileList.size(); i++)
    {
        fileListDivide = fileList[i].split(split2);
        for (int j = 1; j < codeList.size(); j++)
        {
            codeListDivide = codeList[j].split(split2);
            if (codeListDivide[0] == fileListDivide[0])
            {
                if (codeListDivide[1] != fileListDivide[1])
                {
                    QFile file_gitback(codeListDivide[0].append("_back"));
                    if(!file_gitback.open(QFile::WriteOnly | QFile::Text))
                    {
                        qDebug() << " Could not open the file for writing ";
                        return;
                    }
                    QTextStream out_gitback(&file_gitback);
                    out_gitback << codeListDivide[1];
                    file_gitback.flush();
                    file_gitback.close();

                    QFile file(fileListDivide[0]);
                    if(!file.open(QFile::WriteOnly | QFile::Text))
                    {
                        qDebug() << " Could not open the file for writing ";
                        return;
                    }
                    QTextStream out(&file);
                    out << fileListDivide[1];
                    file.flush();
                    file.close();
                }
                break;
            }
            if (j == codeList.size()-1) // ì°¾ìë´¤ëê¸°ì¡´ ì¼ë¤.
            {
                QFile file(fileListDivide[0]);
                if(!file.open(QFile::WriteOnly | QFile::Text))
                {
                    qDebug() << " Could not open the file for writing ";
                    return;
                }
                QTextStream out(&file);
                out << fileListDivide[1];
                file.flush();
                file.close();
            }


        }
    }

}
