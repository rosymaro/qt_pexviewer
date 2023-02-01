#ifndef FORMHIER_H
#define FORMHIER_H

#include <QDialog>

namespace Ui {
class FormHier;
}

class FormHier : public QDialog
{
    Q_OBJECT

public:
    explicit FormHier(QWidget *parent = nullptr);
    ~FormHier();

private:
    Ui::FormHier *ui;
};

#endif // FORMHIER_H
