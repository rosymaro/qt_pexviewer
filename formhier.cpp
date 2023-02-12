#include "formhier.h"
#include "ui_formhier.h"

FormHier::FormHier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormHier)
{
    ui->setupUi(this);
}

void FormHier::slotMove(const QPoint &newPos, const QPoint &oldPos)
{
    auto x = newPos.x() - oldPos.x();
    auto y = newPos.y() - oldPos.y();

    QPoint pos( this ->pos().x() + x, this -> pos().y() + y);
    move(pos);
}

FormHier::~FormHier()
{
    delete ui;
}
