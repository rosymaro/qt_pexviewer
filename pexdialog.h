#ifndef PEXDIALOG_H
#define PEXDIALOG_H

#include <QDialog>

namespace Ui {
class PexDialog;
}

class PexDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PexDialog(QWidget *parent = nullptr);
    ~PexDialog();

private:
    Ui::PexDialog *ui;
};

#endif // PEXDIALOG_H
