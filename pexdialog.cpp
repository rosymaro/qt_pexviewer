#include "pexdialog.h"
#include "ui_pexdialog.h"

PexDialog::PexDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PexDialog)
{
    ui->setupUi(this);
}

PexDialog::~PexDialog()
{
    delete ui;
}
