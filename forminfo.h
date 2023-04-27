#ifndef FORMINFO_H
#define FORMINFO_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidgetItem>
#include <glm/gtx/string_cast.hpp>
#include "T2D.h"
#include "defineParam.h"

namespace Ui {
class FormInfo;
}

class FormInfo : public QDialog
{
    Q_OBJECT

public:
    explicit FormInfo(QWidget *parent = nullptr);
    ~FormInfo();

    void setTextPosTotal(POS_MONITORING *pos);
    void receiveFile(T2D &t2d);
    void receivePointPos(POS_MONITORING &pos);
    void changePos();
    void addItemToPosList(POS_SET *position_item);

private:
    void addItemToListWidget();
    void setTextTrim(QLineEdit *text, double *value, int scale_, QString add_text_);
    void setTextTrim(QLineEdit *text, QString text_);
    void makePosListItem();

    void outputPosList();
    void resetPosList();
    QString makePosText();
    void sendStatusBarMsg(QString msg);
    void inputValueToTemp(double *point, double value_);
    void convertTextToValue(double *point, const QString &str);
    void inputPos();

//    void selectListWidgetItem();

    Ui::FormInfo *ui;
    T2D *rendering_full;
    POS_MONITORING *POS;
    POS_MONITORING TEMP_POS;
    QVector<POS_SET> POS_STACK;
    QList<QListWidgetItem*> selected_item;
    bool isFileOpen = false;
    bool isEndedEdit = true;
    bool make_status_bar_msg = true;
    int current_item = -1;
    int pos_number = 0;
    int item_data_i = 0;

signals:
    void signalDirectlyInputPos();
    void signalStatusBarMsg(QString msg);
private slots:
    void on_pos_x_textEdited(const QString &arg1);
    void on_pos_y_textEdited(const QString &arg1);
    void on_pos_z_textEdited(const QString &arg1);
    void on_ang_tilt_textEdited(const QString &arg1);
    void on_ang_rotation_textEdited(const QString &arg1);
    void on_zoom_textEdited(const QString &arg1);    
    void on_pos_x_editingFinished();
    void on_pos_y_editingFinished();
    void on_pos_z_editingFinished();
    void on_ang_tilt_editingFinished();
    void on_ang_rotation_editingFinished();
    void on_zoom_editingFinished();
    void on_pos_x_returnPressed();
    void on_pos_y_returnPressed();
    void on_pos_z_returnPressed();
    void on_ang_tilt_returnPressed();
    void on_ang_rotation_returnPressed();
    void on_zoom_returnPressed();

    void on_listWidget_currentRowChanged(int currentRow);
    void on_pushButton_go_clicked();
    void on_pushButton_del_clicked();
    void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_listWidget_itemSelectionChanged();
};

#endif // FORMINFO_H
