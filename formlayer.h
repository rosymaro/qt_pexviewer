#ifndef FORMLAYER_H
#define FORMLAYER_H

#include <QDialog>

namespace Ui {
class FormLayer;
}

class FormLayer : public QDialog
{
    Q_OBJECT

public:
    explicit FormLayer(QWidget *parent = nullptr);
    ~FormLayer();

public slots:
    void slotMove(const QPoint &, const QPoint &);

private:
    Ui::FormLayer *ui;
};

#endif // FORMLAYER_H