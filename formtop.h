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
    void receiveFile(QVector<QStringList> strVector);

private:
    Ui::FormTop *ui;
};

#endif // FORMTOP_H
