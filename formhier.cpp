#include "formhier.h"
#include "ui_formhier.h"

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
