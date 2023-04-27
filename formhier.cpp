//#include "/Rendering/Src/LayoutPEXData.h"
#include "formhier.h"
#include "ui_formhier.h"
#include "mainwindow.h"

FormHier::FormHier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormHier)
{
    ui->setupUi(this);

}

FormHier::~FormHier()
{
    delete ui;
}

void FormHier::createHierarchyTree(T2D *t2d){

    this->t2d = t2d;
    vector<QTreeWidgetItem*> m_buf_tree_item;
    QTreeWidgetItem* m_top_tree = new QTreeWidgetItem(ui->hierarchy_tree);
    m_buf_tree_item.resize(99);
    m_buf_tree_item[0] = m_top_tree;
    string m_top_cell_name = this->t2d->HierarchyInstance[0].name + " (" + this->t2d->HierarchyInstance[0].num + ") ";
    m_top_tree->setText(0, QString::fromStdString(m_top_cell_name));
    m_top_tree->setText(1, QString::fromStdString(to_string(0)));
    ui->hierarchy_tree->addTopLevelItem(m_top_tree);

    for(int i = 1 ; i < t2d->HierarchyInstance.size() ; i++){
        int m_level = t2d->HierarchyInstance[i].level;
        string m_cell_name = t2d->HierarchyInstance[i].name + " (" + t2d->HierarchyInstance[i].num + ") ";

        QTreeWidgetItem* m_child_tree = new QTreeWidgetItem();
        m_child_tree->setText(0, QString::fromStdString(m_cell_name));
        m_child_tree->setText(1, QString::fromStdString(to_string(i)));
        m_buf_tree_item[m_level] = m_child_tree;

        m_buf_tree_item[m_level - 1]->addChild(m_child_tree);
    }

//    PEXResistorDataManager::getResistorsOfNode();
//    for (auto& loop : resistors_of_node[0]){
//        qDebug() << "loop test XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
//    }
}

void FormHier::on_hierarchy_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    cout << "minx : " << this->t2d->HierarchyInstance[atoi(item->text(column+1).toStdString().c_str())].box.minx;
    cout << " miny : " << this->t2d->HierarchyInstance[atoi(item->text(column+1).toStdString().c_str())].box.miny;
    cout << " maxx : " << this->t2d->HierarchyInstance[atoi(item->text(column+1).toStdString().c_str())].box.maxx;
    cout << " maxy : " << this->t2d->HierarchyInstance[atoi(item->text(column+1).toStdString().c_str())].box.maxy << endl;

}

void FormHier::on_hierarchy_searching_textEdited(const QString &arg1)
{
//    QString m_input_text = ui->hierarchy_searching->text();
//    QList<QTreeWidgetItem*> m_all_list = ui->hierarchy_tree->findItems("", Qt::MatchContains | Qt::MatchRecursive, 0);
//    if(m_input_text != ""){
//        for (QTreeWidgetItem *item : m_all_list) {
//            item->setTextColor(0, Qt::black);
//        }
//        QList<QTreeWidgetItem*> clist = ui->hierarchy_tree->findItems(m_input_text, Qt::MatchContains | Qt::MatchRecursive, 0);
//        ui->hierarchy_tree->clear();
//        for (QTreeWidgetItem *item : clist) {
//            item->setTextColor(0, Qt::red);
//        }
//    }else{
//        for (QTreeWidgetItem *item : m_all_list) {
//            item->setTextColor(0, Qt::black);
//        }
//    }

//    QTreeWidgetItemIterator it(ui->hierarchy_tree);
//    while(*it){
//        if((*it)->text(0).contains(arg1, Qt::CaseInsensitive)){
//            (*it)->setHidden(false);
//        }else{
//            (*it)->setHidden(true);
//        }
//        ++it;
//    }

//    QSortFilterProxyModel* m_tree_model = new QSortFilterProxyModel(this);
//    m_tree_model->setFilterCaseSensitivity(Qt::CaseInsensitive);
//    m_tree_model->setSourceModel(ui->hierarchy_tree->model());
//    ui->hierarchy_tree->setModel(m_tree_model);

//    m_tree_model->setFilterRegExp(QRegExp(arg1, Qt::CaseInsensitive, QRegExp::FixedString));
    QString m_hierarchy_searching_text = arg1;

    if(m_hierarchy_searching_text.contains('*')){
        m_hierarchy_searching_text.replace("*", ".*", Qt::CaseInsensitive);
        QRegExp m_regex(m_hierarchy_searching_text, Qt::CaseInsensitive);
        QTreeWidgetItemIterator it(ui->hierarchy_tree);
        while(*it){
            if(m_regex.exactMatch((*it)->text(0))){
                QTreeWidgetItem* item = *it;
                while(item){
                    item->setExpanded(true);
                    item->setHidden(false);
                    item = item->parent();
                }
            }else{
                (*it)->setHidden(true);
            }
            ++it;
        }
    } else{
        QTreeWidgetItemIterator it(ui->hierarchy_tree);
        while(*it){
            if((*it)->text(0).contains(m_hierarchy_searching_text, Qt::CaseInsensitive)){
                QTreeWidgetItem* item = *it;
                while(item){
                    item->setExpanded(true);
                    item->setHidden(false);
                    item = item->parent();
                }
            }else{
                (*it)->setHidden(true);
            }
            ++it;
        }
    }
}












