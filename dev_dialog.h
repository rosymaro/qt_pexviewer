#ifndef DEV_DIALOG_H
#define DEV_DIALOG_H

#include <QDialog>
#include <defineParam.h>


#include "Rendering/Src/lve_window.hpp"
#include "Rendering/Src/dtaorendersystem.h"

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

    void on_pushButton_initCamera_clicked();

private:
    Ui::DevDialog *ui;
    bool *is_pos_checked;
    bool init_check = false;
    POS_MONITORING *pos;

    LveWindow* m_window = nullptr;

public:
    void setRenderWindow(LveWindow* window_){ this->m_window = window_;}

private:
    DtaoRenderSystem* getRenderer(){ return this->m_window->getRenderer();}

};

#endif // DEV_DIALOG_H
