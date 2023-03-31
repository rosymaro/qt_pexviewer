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

    readCodeFiles();
    readTxtFile();
    if(write_code_files)
    {
        makeCodeFiles();
    }
    else
    {
        makeTxtFile();
    }

}

void GitMerge::readTxtFile()
{
    QString fileName = txt_file;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading ";
        based_on_reading_txt = "";
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
    based_on_reading_txt = in.readAll();
    file.close();

}

void GitMerge::readCodeFiles()
{
    QFile file(pro_file);


    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for reading ";
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString merge = in.readAll();
    QStringList mergeList = merge.split("\n");
    for (auto &itemMerge : mergeList)
    {

        itemMerge = itemMerge.remove("\\");
        itemMerge = itemMerge.trimmed();

        if (itemMerge.contains(".cpp", Qt::CaseInsensitive) || itemMerge.contains(".h", Qt::CaseInsensitive) || itemMerge.contains(".ui", Qt::CaseInsensitive))
        {
            QFile codeFile(itemMerge.prepend("./"));
            if(!codeFile.open(QFile::ReadOnly | QFile::Text))
            {
                qDebug() << " Could not open the code file for reading " ;
                return;
            }
            QTextStream codeIn(&codeFile);
            codeIn.setCodec("UTF-8");
            QString codeText = codeIn.readAll();
            codeText = codeText.toUtf8();

            merged_code_files.append(split1);
            merged_code_files.append(itemMerge);
            merged_code_files.append(split2);
            merged_code_files.append(codeText);
            codeFile.close();

        }

    }
    merged_code_files.append(split1);
    merged_code_files.append("./IInterface.pro");
    merged_code_files.append(split2);
    merged_code_files.append(merge);

    file.close();
}

bool GitMerge::compareCodeFile(QString &A, QString &B)
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

void GitMerge::makeTxtFile()
{
    QFile file(txt_file);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << " Could not open the file for writing ";
        return;
    }
    QTextStream out(&file);
    out << merged_code_files;
    out.setCodec("UTF-8");
    file.flush();
    file.close();

}

void GitMerge::makeCodeFiles()
{
    QStringList codeList = merged_code_files.split(split1);
    QStringList fileList = based_on_reading_txt.split(split1);
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
                    out_gitback.setCodec("UTF-8");
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
                    out.setCodec("UTF-8");
                    file.flush();
                    file.close();
                }
                break;
            }
            if (j == codeList.size()-1) // 찾아봤는데 기존 파일이 없다.
            {
                QFile file(fileListDivide[0]);
                if(!file.open(QFile::WriteOnly | QFile::Text))
                {
                    qDebug() << " Could not open the file for writing ";
                    return;
                }
                QTextStream out(&file);
                out << fileListDivide[1];
                out.setCodec("UTF-8");
                file.flush();
                file.close();
            }
        }
    }
}
