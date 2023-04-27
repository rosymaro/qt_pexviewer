#ifndef DEV_DIALOG_H
#define DEV_DIALOG_H

#include <QDialog>
#include <defineParam.h>

namespace Ui {
class DevDialog;
}

class DevDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DevDialog(QWidget *parent = nullptr);
    ~DevDialog();

    void sendParameter(bool *is_pos_checked_, POS_MONITORING *pos_);
    void changePos();

private slots:
    void on_checkBox_pos_stateChanged(int arg1);

private:
    Ui::DevDialog *ui;
    bool *is_pos_checked;
    bool init_check = false;
    POS_MONITORING *pos;
};

#endif // DEV_DIALOG_H
