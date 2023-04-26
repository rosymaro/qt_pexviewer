#ifndef FORMHIER_H
#define FORMHIER_H

#include <QDialog>
//#include "all_data.h"
//#include "mainwindow.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSortFilterProxyModel>
#include <vector>
#include <iostream>

#include "T2D.h"

namespace Ui {
class FormHier;
}

class FormHier : public QDialog
{
    Q_OBJECT

public:
    FormHier(QWidget *parent = nullptr);
    ~FormHier();
    Ui::FormHier *ui;

    T2D *t2d;

    void createHierarchyTree(T2D *t2d);
private slots:
    void on_hierarchy_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_hierarchy_searching_textEdited(const QString &arg1);
};

#endif // FORMHIER_H

/////
