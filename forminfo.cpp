#include "forminfo.h"
#include "ui_forminfo.h"
#include <QDebug>
#include "cmath"

FormInfo::FormInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormInfo)
{
    ui->setupUi(this);
    ui->pointX->setText(QString::number(round(pointX*10000)/10000));
    ui->pointY->setText(QString::number(round(pointY*10000)/10000));
    ui->pointZ->setText(QString::number(round(pointZ*10000)/10000));
    ui->infoRot->setText(QString::number(round(infoRot*10000)/10000));
    ui->infoTilt->setText(QString::number(round(infoTilt*10000)/10000));
    ui->infoZoom->setText(QString::number(round(1/infoZoom*1000)/10)+"%");
}

void FormInfo::slotInfoText(QString funcName, float value)
{


    if (funcName == "moveGdsX")
    {
        pointX = pointX + value;
        ui->pointX->setText(QString::number(round(pointX*10000)/10000));
        initPointX = pointX;
    }
    if (funcName == "moveGdsY")
    {
        pointY = pointY + value;
        ui->pointY->setText(QString::number(round(pointY*10000)/10000));
        initPointY = pointY;
    }
    if (funcName == "moveGdsZ")
    {
        pointZ = pointZ + value;
        ui->pointZ->setText(QString::number(round(pointZ*10000)/10000));
        initPointZ = pointZ;
    }
    if (funcName == "rotateRenderX")
    {
        infoRot = initRot + value;
        if (infoRot>360)
            infoRot = infoRot - 360;
        if (infoRot<0)
            infoRot = infoRot + 360;
        ui->infoRot->setText(QString::number(round(infoRot*10000)/10000));
    }
    if (funcName == "rotateRenderY")
    {
        qDebug()<<"rotateRenderY";
        infoTilt = initTilt + value;
        if (infoTilt>90)
            infoTilt = 90;
        if (infoTilt<-90)
            infoTilt = -90;
        ui->infoTilt->setText(QString::number(round(infoTilt*10000)/10000));
    }
    if (funcName == "moveZoom")
    {
        qDebug()<<"moveZoom";
        infoZoom = infoZoom + infoZoom*value/1500;        //일정 높이를 100% 로 설정할 필요 있음, 결국 초기값은 GDS Size 에 따라 바뀔 수 있음.
        if (infoZoom < 0.01)
            infoZoom = 0.01;
        if (infoZoom > 100)
            infoZoom = 100;
        ui->infoZoom->setText(QString::number(round(1/infoZoom*1000)/10)+"%");
    }
    if (funcName == "moveRenderX")
    {
        qDebug()<< "moveRenderX";
        x = value;
        moveRender(x,y);
//        pointX = initPointX+value*cos(infoRot*pi/180)*infoZoom;
//        pointY = initPointY+value*sin(infoRot*pi/180)*infoZoom;
//        qDebug() << pointX << " : " << pointY;
//        ui->pointX->setText(QString::number(round(pointX*10000)/10000));
//        ui->pointY->setText(QString::number(round(pointY*10000)/10000));
    }
    if (funcName == "moveRenderY")
    {
        qDebug()<< "moveRenderY";
        y = value;
        moveRender(x,y);
//        pointX = initPointX+value*sin(infoRot*pi/180)*sin(infoTilt*pi/180)*infoZoom;
//        pointY = initPointY+value*cos(infoRot*pi/180)*sin(infoTilt*pi/180)*infoZoom;
//        pointZ = initPointZ+value*cos(infoTilt*pi/180)*infoZoom;
//        qDebug() << pointX << " : " << pointY;
//        ui->pointX->setText(QString::number(round(pointX*10000)/10000));
//        ui->pointY->setText(QString::number(round(pointY*10000)/10000));
//        ui->pointZ->setText(QString::number(round(pointZ*10000)/10000));
    }
    if (funcName == "mouseRelease")
    {
        qDebug()<< "mouseRelease";
        initTilt = infoTilt;
        initRot = infoRot;
        initPointX = pointX;
        initPointY = pointY;
        initPointZ = pointZ;

    }

}

void FormInfo::moveRender(float x, float y)
{
    const double pi = 3.1415926;

    qDebug()<< "moveRender";
    pointX = initPointX+x*cos(infoRot*pi/180)*infoZoom + y*sin(infoRot*pi/180)*sin(infoTilt*pi/180)*infoZoom;
    pointY = initPointY+x*sin(infoRot*pi/180)*infoZoom + y*cos(infoRot*pi/180)*sin(infoTilt*pi/180)*infoZoom;
    pointZ = initPointZ+y*cos(infoTilt*pi/180)*infoZoom;

    ui->pointX->setText(QString::number(round(pointX*10000)/10000));
    ui->pointY->setText(QString::number(round(pointY*10000)/10000));
    ui->pointZ->setText(QString::number(round(pointZ*10000)/10000));

}

FormInfo::~FormInfo()
{
    delete ui;
}
