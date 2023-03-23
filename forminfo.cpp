#include "forminfo.h"
#include "ui_forminfo.h"
#include <QDebug>
#include "cmath"

FormInfo::FormInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormInfo)
{
    ui->setupUi(this);

    QString init_x = "";
    QString init_y = "";
    QString init_z = "";
    QString init_t = "";
    QString init_r = "";
    QString init_m = "";

    ui->pos_x->setText(init_x);
    ui->pos_y->setText(init_y);
    ui->pos_z->setText(init_z);
    ui->ang_tilt->setText(init_t);
    ui->ang_rotation->setText(init_r);
    ui->zoom->setText(init_m);
}

FormInfo::~FormInfo()
{
    delete ui;
}

void FormInfo::receiveFile(T2D &t2d)
{
    rendering_full = &t2d;
    m_min_x = t2d.LayoutMinMax.minx;
    m_max_x = t2d.LayoutMinMax.maxx;
    m_min_y = t2d.LayoutMinMax.miny;
    m_max_y = t2d.LayoutMinMax.maxy;
    m_min_z = t2d.LayoutMinMax.minz;
    m_max_z = t2d.LayoutMinMax.maxz;
}

void FormInfo::outputText()
{
    ui->pos_x->setText(QString::number(round(pos->x*10000)/10000));
    ui->pos_y->setText(QString::number(round(pos->y*10000)/10000));
    ui->pos_z->setText(QString::number(round(pos->z*10000)/10000));
    ui->ang_tilt->setText(QString::number(round(pos->tilt*10000)/10000));
    ui->ang_rotation->setText(QString::number(round(pos->rotation*10000)/10000));
    ui->zoom->setText(QString::number(round(pos->zoom))+"%");
}

void FormInfo::slotInfoText(QString funcName, POS_MONITORING value)
{
    if (funcName == "camera_position")
    {
        float rot = value.rotation;
        if(rot <0) {
            rot = rot+360;
        }
        ui->ang_rotation->setText(QString::number(round(rot)));
        ui->ang_tilt->setText(QString::number(round(value.tilt)));
        ui->zoom->setText(QString::number(round(value.zoom*100)));

        emit signalPos();
    }
}

void FormInfo::slotPos(POS_MONITORING &pos)
{
    this->pos = &pos;
    temp_pos.x = pos.x;
    temp_pos.y = pos.y;
    temp_pos.z = pos.z;
    temp_pos.tilt = pos.tilt;
    temp_pos.rotation = pos.rotation;
    temp_pos.zoom = pos.zoom;
    temp_pos.window_zoom = 1;
}

void FormInfo::moveRender(float x, float y)
{
    const double pi = 3.1415926;

    qDebug()<< "moveRender";
    pointX = initPointX+x*cos(infoRot*pi/180)*infoZoom + y*sin(infoRot*pi/180)*sin(infoTilt*pi/180)*infoZoom;
    pointY = initPointY+x*sin(infoRot*pi/180)*infoZoom + y*cos(infoRot*pi/180)*sin(infoTilt*pi/180)*infoZoom;
    pointZ = initPointZ+y*cos(infoTilt*pi/180)*infoZoom;

    ui->pos_x->setText(QString::number(round(pointX*10000)/10000));
    ui->pos_y->setText(QString::number(round(pointY*10000)/10000));
    ui->pos_z->setText(QString::number(round(pointZ*10000)/10000));

}

bool FormInfo::checkNum(const QString &str)
{
    if(str.toDouble() == 0 && str != "0")
    // «ìë¡ë°ê¿¨ë 0ìë° string  0 ë ê²½ì°ë ë¬¸ì
    {
        return false;
    }
    else
    {
        return true;
    }
}

void FormInfo::checkText(double *point, const QString &arg1, double min_limit, double max_limit)
{
    double double_arg = arg1.toDouble();
    if(checkNum(arg1)) // «ì¸ì ì
    {
        if(double_arg >= min_limit && double_arg <= max_limit) //tilt
        {
            *point = double_arg;
        }
    }
    else if(arg1.isEmpty() || arg1.contains("-") || arg1.contains("0."))
    {

    }    
    else // ê¸°ì¡´ ê°ì¼ë¡ ì
    {
        outputText();
    }
}


void FormInfo::on_pos_x_textEdited(const QString &arg1)
{
    double min_limit = m_min_x;
    double max_limit = m_max_x;
    checkText(&temp_pos.x,arg1,min_limit,max_limit);
}

void FormInfo::on_pos_y_textEdited(const QString &arg1)
{
    double min_limit = m_min_y;
    double max_limit = m_max_y;
    checkText(&temp_pos.y,arg1,min_limit,max_limit);

}

void FormInfo::on_pos_z_textEdited(const QString &arg1)
{
    double min_limit = m_min_z;
    double max_limit = m_max_z;
    checkText(&temp_pos.z,arg1,min_limit,max_limit);
}

void FormInfo::on_ang_tilt_textEdited(const QString &arg1)
{
    double min_limit = m_min_tilt;
    double max_limit = m_max_tilt;
    checkText(&temp_pos.tilt,arg1,min_limit,max_limit);
}

void FormInfo::on_ang_rotation_textEdited(const QString &arg1)
{
    double min_limit = m_min_rot;
    double max_limit = m_max_rot;
    checkText(&temp_pos.rotation,arg1,min_limit,max_limit);
}

void FormInfo::on_zoom_textEdited(const QString &arg1)
{
    double min_limit = m_min_zoom;
    double max_limit = m_max_zoom;
    checkText(&temp_pos.zoom,arg1,min_limit,max_limit);
}

void FormInfo::inputPos()
{
    pos->x = temp_pos.x;
    pos->y = temp_pos.y;
    pos->z = temp_pos.z;
    pos->tilt = temp_pos.tilt;
    pos->rotation = temp_pos.rotation;
    pos->zoom = temp_pos.zoom;
    qDebug()<< "inputPos : " << pos->x << ","
                             << pos->y << ","
                             << pos->z << ","
                             << pos->tilt << ","
                             << pos->rotation << ","
                             << pos->zoom;
    emit signalPos();
}

void FormInfo::on_pos_x_returnPressed()
{
    inputPos();
}

void FormInfo::on_pos_y_returnPressed()
{
    inputPos();
}

void FormInfo::on_pos_z_returnPressed()
{
    inputPos();
}

void FormInfo::on_ang_tilt_returnPressed()
{
    inputPos();
}

void FormInfo::on_ang_rotation_returnPressed()
{
    inputPos();
}

void FormInfo::on_zoom_returnPressed()
{
    inputPos();
}

void FormInfo::on_pos_x_editingFinished()
{
    ui->pos_x->setText(QString::number(round(temp_pos.x*10000)/10000));
}

void FormInfo::on_pos_y_editingFinished()
{
    ui->pos_y->setText(QString::number(round(temp_pos.y*10000)/10000));
}

void FormInfo::on_pos_z_editingFinished()
{
    ui->pos_z->setText(QString::number(round(temp_pos.z*10000)/10000));
}

void FormInfo::on_ang_tilt_editingFinished()
{
    ui->ang_tilt->setText(QString::number(round(temp_pos.tilt*10000)/10000));
}

void FormInfo::on_ang_rotation_editingFinished()
{
    ui->ang_rotation->setText(QString::number(round(temp_pos.rotation*10000)/10000));
}

void FormInfo::on_zoom_editingFinished()
{
    ui->zoom->setText(QString::number(round(pos->zoom))+"%");
}
