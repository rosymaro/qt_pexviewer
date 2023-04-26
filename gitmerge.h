#ifndef GITMERGE_H
#define GITMERGE_H

#include <QString>

class GitMerge
{
public:
    GitMerge();
private:
    void readTxtFile();
    void readCodeFiles();
    bool compareCodeFile(QString &A, QString &B);
    void makeTxtFile();
    void makeCodeFiles();

    bool write_code_files = false;
    QString split1;
    QString split2;
    QString merged_code_files;    //媛곸쥌 cpp, h 깆뿉쎌뼱댁슜
    QString based_on_reading_txt; //gitMerge.txt 먯꽌 쎌뼱댁슜
    QString txt_file = "./gitMerge.txt";
    QString pro_file = "./IInterface.pro";
    QString hash = "!@hash[]";
    QString file = "!@file[]";
    QString code = "!@code[]";
};

#endif // GITMERGE_H
