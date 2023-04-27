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
    void getScaleValue(double *m_scale_, double *m_from_window_to_box_x_, double *m_from_window_to_box_y_, double *m_min_x_, double *m_min_y_, double *m_zoom_init_);
public slots:
    void slotMove(POS_MONITORING *pos);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);    
    QRectF boundingRect() const;


//    virtual void keyPressEvent(QKeyEvent *evnet);

private:
    bool init_check = true;
    double convertPosX();
    double convertPosY();
    double m_pos_past_x = 0;
    double m_pos_past_y = 0;
    double m_view_size = 1;    
    double m_gray_box_size = 120;
    double *m_scale;
    double *m_zoom_init;
    double *m_from_window_to_box_x;
    double *m_from_window_to_box_y;
    double *m_min_x;
    double *m_min_y;
    double m_zero_point_x;
    double m_zero_point_y;
    double temp_x;      // 마우스로 box 움직일 때 좌표 알아보기 위함
    double temp_y;
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
    void signalMove(POS_MONITORING *pos);
    void signalInitMove(double x, double y);

private:    
    Ui::FormMap *ui;
    QGraphicsRectItem *rectItem;
    QGraphicsRectItem *rectItem2;
    QRect *rect;
    double m_min_x;
    double m_min_y;
    double m_scale = 1;
    double m_zoom_init;
    double m_gray_box_size = 120;
    double m_window_width;
    double m_window_height;
    double m_box_width;
    double m_box_height;
    double m_from_window_to_box_x;
    double m_from_window_to_box_y;

    SuperItem *super;

    POS_MONITORING *pos;

    float pointX = 0;
    float pointY = 0;
    float pointZ = 0;
    float infoTilt = 90;
    float infoRot = 0;
    float infoZoom = 1; //Zoom 초기값에 대한 GDS size 를 갖고와 수정 필요

    float initRot = 0;
    float initTilt = 90;
    float initPointX = 0;
    float initPointY = 0;
    float initPointZ = 0;

    float x = 0;
    float y = 0;
};



#endif // FORMMAP_H
