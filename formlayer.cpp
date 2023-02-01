#include "formlayer.h"
#include "ui_formlayer.h"

FormLayer::FormLayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLayer)
{
    ui->setupUi(this);
}

FormLayer::~FormLayer()
{
    delete ui;
}
