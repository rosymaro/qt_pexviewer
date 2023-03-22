#ifndef GITMERGE_H
#define GITMERGE_H

#include <QString>

class GitMerge
{
public:
    GitMerge();
private:
    void mergeFiles(QString &totalMerge);
    void openPreMergeFile(QString &preMergeFile);
    void openUpdateMergeFile(QString &updateMergeFile);
    bool compareFiles(QString &A, QString &B);
    void writeMergeFile(QString fileName, QString &originFile);
    void makeCodeFiles(QString &codeFile, QString &gitMergeFile);

    QString split1;
    QString split2;
    QString hash = "!@hash[]";
    QString file = "!@file[]";
    QString code = "!@code[]";
};

#endif // GITMERGE_H
