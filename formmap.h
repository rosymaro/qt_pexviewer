#ifndef FORMMAP_H
#define FORMMAP_H

#include <QDialog>

namespace Ui {
class FormMap;
}

class FormMap : public QDialog
{
    Q_OBJECT

public:
    explicit FormMap(QWidget *parent = nullptr);
    ~FormMap();

private:
    Ui::FormMap *ui;
};

#endif // FORMMAP_H
