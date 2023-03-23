#ifndef FORMMAP_H
#define FORMMAP_H
#include <glm/gtx/string_cast.hpp>
#include <QDialog>
#include <QGraphicsRectItem>
#include "defineParam.h"

namespace Ui {
class FormMap;
}

class SuperItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    SuperItem(QGraphicsItem* parent = NULL);
public slots:
    void slotMove(float x, float y, float zoom, float rot, float tilt);    

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QRectF boundingRect() const;

//    virtual void keyPressEvent(QKeyEvent *evnet);
    float _zoom = 1;
    float _rot = 0;
    float _tilt = 90;

};

class FormMap : public QDialog
{
    Q_OBJECT

public:
    explicit FormMap(QWidget *parent = nullptr);
    ~FormMap();
    void receiveSize(float &xMinSize,float &yMinSize,float &xMaxSize,float &yMaxSize, float &zoomScale);
    void receiveSize_t2d(double &xMinSize,double &yMinSize,double &xMaxSize,double &yMaxSize, double &zoomScale);

public slots:
    void slotInfoText(QString funcName, POS_MONITORING value);

signals:
    void signalMove(float x, float y, float zoom, float rot, float tilt);

private:
    Ui::FormMap *ui;
    QGraphicsRectItem *rectItem;
    QGraphicsRectItem *rectItem2;
    QRect *rect;
    float width;
    float height;
    SuperItem *super;

    float pointX = 0;
    float pointY = 0;
    float pointZ = 0;
    float infoTilt = 90;
    float infoRot = 0;
    float infoZoom = 1; //Zoom ì´ê¸°ê°ì GDS size ë¥ê°ê³  ì  ì

    float initRot = 0;
    float initTilt = 90;
    float initPointX = 0;
    float initPointY = 0;
    float initPointZ = 0;

    float x = 0;
    float y = 0;
};



#endif // FORMMAP_H
