#include "formmap.h"
#include "ui_formmap.h"

FormMap::FormMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormMap)
{
    ui->setupUi(this);
}

FormMap::~FormMap()
{
    delete ui;
}
