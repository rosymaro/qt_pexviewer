#ifndef FORMINFO_H
#define FORMINFO_H

#include <QDialog>
#include <QLineEdit>
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
    void receiveFile(T2D &t2d);
    void outputText();
    void receivePointPos(POS_MONITORING &pos);
    void changePos();

public slots:
    void moveRender(float x, float y);

private:
    bool checkNum(const QString &str);
    void checkText(double *point, const QString &str, double min_limit, double max_limit, int type);

    T2D *rendering_full;
    Ui::FormInfo *ui;
    POS_MONITORING *pos;
    POS_MONITORING temp_pos;
    float pointX = 0;
    float pointY = 0;
    float pointZ = 0;
    float infoTilt = 90;
    float infoRot = 0;
    float infoZoom = 1; //Zoom ì´ê¸°ê°ì GDS size ë¥ê°ê³  ì  ì

    float initRot = 0;
    float initTilt = 90;
    float initPointX = 0;
    float initPointY = 0;
    float initPointZ = 0;

    float x = 0;
    float y = 0;

    double m_temp_x = 0;
    double m_temp_y = 0;
    double m_temp_z = 0;
    double m_temp_tilt = 0;
    double m_temp_rot = 0;
    double m_temp_zoom = 0;

    double m_min_x = 0;
    double m_max_x = 0;
    double m_min_y = 0;
    double m_max_y = 0;
    double m_min_z = 0;
    double m_max_z = 0;
    double m_min_tilt = -90;
    double m_max_tilt = 90;
    double m_min_rot = 0;
    double m_max_rot = 360;
    double m_min_zoom = 0.01;
    double m_max_zoom = 999;

signals:
    void signalDirectlyInputPos();
private slots:
    void on_pos_x_textEdited(const QString &arg1);
    void on_pos_y_textEdited(const QString &arg1);
    void on_pos_z_textEdited(const QString &arg1);
    void on_ang_tilt_textEdited(const QString &arg1);
    void on_ang_rotation_textEdited(const QString &arg1);
    void on_zoom_textEdited(const QString &arg1);
    void inputPos();
    void on_pos_x_returnPressed();
    void on_pos_y_returnPressed();
    void on_pos_z_returnPressed();
    void on_ang_tilt_returnPressed();
    void on_ang_rotation_returnPressed();
    void on_zoom_returnPressed();
    void on_pos_x_editingFinished();
    void on_pos_y_editingFinished();
    void on_pos_z_editingFinished();
    void on_ang_tilt_editingFinished();
    void on_ang_rotation_editingFinished();
    void on_zoom_editingFinished();

};

#endif // FORMINFO_H
