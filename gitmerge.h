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
};

#endif // GITMERGE_H
