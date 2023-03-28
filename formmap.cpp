#include "formmap.h"
#include "ui_formmap.h"
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QVector>
#include <iostream>
#include <QDebug>
#include <QRect>
#include <QKeyEvent>

SuperItem::SuperItem(QGraphicsItem* parent) : QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
}

void SuperItem::receivePointPos(POS_MONITORING *pos)
{
    this->m_pos = pos;
}

void SuperItem::slotMove()
{
    qDebug() << "1 pos delta x :" << m_pos->x << " || delta y :" << m_pos->y;
    qDebug() << "2 pas delta x :" << m_pos_past_x << " || delta y :" << m_pos_past_y;
    double delta_x = m_scale*(m_pos->x - m_pos_past_x);
    double delta_y = m_scale*(m_pos->y - m_pos_past_y);
    double rot = 360 - m_pos->rotation;

    if (delta_x !=0 || delta_y !=0)
    {
        qDebug() << "delta x :" << delta_x << " || delta y :" << delta_y;
        moveBy(delta_x, -1 * delta_y);
        m_pos_past_x = m_pos->x;
        m_pos_past_y = m_pos->y;
    }



    QTransform trans;
    trans.rotate(rot);
    trans.scale(m_view_size*m_zoom_init/m_pos->zoom, (m_view_size*m_zoom_init/m_pos->zoom) * (m_pos->tilt / 90));
      //zoom 초기가 25%면 ..
    setTransform(trans);

}

void SuperItem::slotInitMove(double x, double y, double scale)
{
    m_pos_past_x = m_pos->x;
    m_pos_past_y = m_pos->y;
    m_scale = scale;
    moveBy(x,y);
}

void SuperItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
    painter->drawRoundedRect(-88,-88,175,175,5,5);
    painter->setPen(Qt::blue);
    painter->drawLine(-86,-86,86,-86);
}

QRectF SuperItem::boundingRect() const{
    return QRectF(-89,-89,176,176);
}



FormMap::FormMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormMap)
{
    ui->setupUi(this);
    super = new SuperItem;
}

FormMap::~FormMap()
{
    delete ui;
}

void FormMap::receiveFile(T2D &t2d)
{
    m_width = t2d.LayoutMinMax.maxx - t2d.LayoutMinMax.minx;
    m_height = t2d.LayoutMinMax.maxy - t2d.LayoutMinMax.miny;
    m_scale = m_box_size/qMax(m_width,m_height);
    m_width_scaled = m_scale * m_width;
    m_height_scaled = m_scale * m_height;
    double window_width = ui->graphicsView->size().width();
    double window_height = ui->graphicsView->size().height();

    QGraphicsRectItem *rectItem = new QGraphicsRectItem;
    QGraphicsScene *scene = new QGraphicsScene(this);
    rectItem->setRect(-(m_width_scaled/2),-(m_height_scaled/2),m_width_scaled,m_height_scaled);
    rectItem->setBrush(QBrush(QColor(Qt::gray)));
    rectItem->setPos(window_width/2,-window_height/2); //좌측 위 (0,0) 에서 부터 중앙으로 이동
    scene->addItem(rectItem);
    scene->setSceneRect(0,-m_box_size,m_box_size,m_box_size);
            //

    super = new SuperItem;
    scene->addItem(super);

    QObject::connect(this,&FormMap::signalMove,super,&SuperItem::slotMove);
    QObject::connect(this,&FormMap::signalInitMove,super,&SuperItem::slotInitMove);

    emit signalInitMove(window_width/2,-window_height/2,m_scale);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(m_box_size,m_box_size);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

}

void FormMap::receivePointPos(POS_MONITORING &pos)
{
    this->pos = &pos;
    super->receivePointPos(this->pos);
}
void FormMap::changePos()
{
    emit signalMove();

}




    //if (funcName == "moveGdsX")
    //{
    //    pointX = pointX + value;
    //    initPointX = pointX;
    //    emit signalMove(value,0,0,999,999);
    //}
    //if (funcName == "moveGdsY")
    //{
    //    pointY = pointY + value;
    //    initPointY = pointY;
    //    emit signalMove(0, value,0,999,999);
    //}
    //if (funcName == "rotateRenderX")
    //{
    //    infoRot = initRot + value;
    //    if (infoRot>360)
    //        infoRot = infoRot - 360;
    //    if (infoRot<0)
    //        infoRot = infoRot + 360;
    //    emit signalMove(0, 0 ,0,infoRot,999);
    //}
    //if (funcName == "rotateRenderY")
    //{
    //    infoTilt = initTilt + value;
    //    if (infoTilt>90)
    //        infoTilt = 90;
    //    if (infoTilt<-90)
    //        infoTilt = -90;
    //    emit signalMove(0, 0 ,0,999,infoTilt);
    //}
    //if (funcName == "moveZoom")
    //{
    //    infoZoom = infoZoom + infoZoom*value/1500;        //일정 높이를 100% 로 설정할 필요 있음, 결국 초기값은 GDS Size 에 따라 바뀔 수 있음.
    //    if (infoZoom < 0.01)
    //        infoZoom = 0.01;
    //    if (infoZoom > 100)
    //        infoZoom = 100;
    //    emit signalMove(0, 0, infoZoom, 999, 999);
    //}
    //if (funcName == "moveRenderX")
    //{
    //    x = value;
    //}
    //if (funcName == "moveRenderY")
    //{
    //    y = value;
    //}
    //if (funcName == "mouseRelease")
    //{
    //    initTilt = infoTilt;
    //    initRot = infoRot;
    //    initPointX = pointX;
    //    initPointY = pointY;
    //    initPointZ = pointZ;
    //}
//}

//void SuperItem::keyPressEvent(QKeyEvent *event){
//    switch(event->key()){
//    case Qt::Key_D:{
//        moveBy(30,0);
//        qDebug() << "press D";
//        break;
//        }
//    case Qt::Key_A:{
//        moveBy(-30,0);
//        break;
//        }
//    case Qt::Key_W:{
//        moveBy(0,-30);
//        break;
//        }
//    case Qt::Key_S:{
//        moveBy(0,30);
//        break;
//        }
//    }
//    update();

//}
