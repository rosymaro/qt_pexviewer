#include "forminfo.h"
#include "ui_forminfo.h"

FormInfo::FormInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormInfo)
{
    ui->setupUi(this);
}

FormInfo::~FormInfo()
{
    delete ui;
}
