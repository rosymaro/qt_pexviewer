#ifndef FORMMAP_H
#define FORMMAP_H
#include <glm/gtx/string_cast.hpp>
#include <QDialog>
#include <QGraphicsRectItem>
#include "defineParam.h"
#include "T2D.h"

namespace Ui {
class FormMap;
}

class SuperItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    SuperItem(QGraphicsItem* parent = NULL);
    void receivePointPos(POS_MONITORING *pos);
public slots:
    void slotMove();
    void slotInitMove(double x, double y, double scale);

protected:    
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);    
    QRectF boundingRect() const;


//    virtual void keyPressEvent(QKeyEvent *evnet);

private:
    POS_MONITORING *m_pos;
    double m_pos_past_x = 0;
    double m_pos_past_y = 0;
    double m_view_size = 1;
    double m_zoom_init = 1;
    double m_init_x = 0;
    double m_init_y = 0;
    double m_scale = 1;
};

class FormMap : public QDialog
{
    Q_OBJECT

public:
    explicit FormMap(QWidget *parent = nullptr);
    ~FormMap();
    void receiveFile(T2D &t2d);
    void receivePointPos(POS_MONITORING &pos);
    void changePos();

signals:
    void signalMove();
    void signalInitMove(double x, double y, double m_scale);


private:    
    Ui::FormMap *ui;
    QGraphicsRectItem *rectItem;
    QGraphicsRectItem *rectItem2;
    QRect *rect;
    double m_width;
    double m_height;
    double m_scale = 1;
    double m_box_size = 178;
    double m_width_scaled;
    double m_height_scaled;

    SuperItem *super;

    POS_MONITORING *pos;

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
};



#endif // FORMMAP_H
