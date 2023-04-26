#include "dev_dialog.h"
#include "ui_dev_dialog.h"
#include <QDebug>


DevDialog::DevDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevDialog)
{
    ui->setupUi(this);
    ui->checkBox_pos->setChecked(true);
}

DevDialog::~DevDialog()
{
    delete ui;
}

void DevDialog::sendParameter(bool *is_pos_checked_, POS_MONITORING *pos_)
{
    is_pos_checked = is_pos_checked_;
    pos = pos_;
    init_check = true;
}


void DevDialog::changePos()
{
//    QString position;
//    position = "(" + QString::number(pos->x) + "," + QString::number(pos->y) + "," + QString::number(pos->z) + ")";
//    ui->label_pos->setText(position);
}

void DevDialog::on_checkBox_pos_stateChanged(int arg1)
{
    if(init_check)
    {
        if(ui->checkBox_pos->isChecked())
        {
            *is_pos_checked = true;
        }
        else
        {
            *is_pos_checked = false;
        }
    }
}

void DevDialog::on_pushButton_initCamera_clicked()
{
    DtaoRenderSystem* renderer = getRenderer();
    renderer->initCameraView();
}
