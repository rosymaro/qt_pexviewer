#include "formtop.h"
#include "ui_formtop.h"

FormTop::FormTop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormTop)
{
    ui->setupUi(this);
}

FormTop::~FormTop()
{
    delete ui;
}
