#include "formlayer.h"
#include "ui_formlayer.h"

FormLayer::FormLayer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormLayer)
{
    ui->setupUi(this);
}

void FormLayer::slotMove(const QPoint &newPos, const QPoint &oldPos)
{
    auto x = newPos.x() - oldPos.x();
    auto y = newPos.y() - oldPos.y();

    QPoint pos( this ->pos().x() + x, this -> pos().y() + y);
    move(pos);
}

FormLayer::~FormLayer()
{
    delete ui;
}
