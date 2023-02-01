#ifndef FORMTOP_H
#define FORMTOP_H

#include <QDialog>

namespace Ui {
class FormTop;
}

class FormTop : public QDialog
{
    Q_OBJECT

public:
    explicit FormTop(QWidget *parent = nullptr);
    ~FormTop();

private:
    Ui::FormTop *ui;
};

#endif // FORMTOP_H
