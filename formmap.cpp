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

void SuperItem::getScaleValue(double *m_scale_, double *m_from_window_to_box_x_, double *m_from_window_to_box_y_, double *m_min_x_, double *m_min_y_, double *m_zoom_init_)
{
    m_scale = m_scale_;
    m_from_window_to_box_x = m_from_window_to_box_x_;
    m_from_window_to_box_y = m_from_window_to_box_y_;
    m_min_x = m_min_x_;
    m_min_y = m_min_y_;
    m_zoom_init = m_zoom_init_;
}

void SuperItem::slotMove(POS_MONITORING *pos)
{    
    //emit signalInitMove(window_width/2,-window_height/2);
    double delta_x = (pos->x - convertPosX()) * (*m_scale);
    double delta_y = (pos->y - convertPosY()) * (*m_scale);
    double rot = 360 - pos->rotation;

    if (abs(delta_x) >__DBL_EPSILON__ || abs(delta_y) > __DBL_EPSILON__) // 0보다 크면
    {        
        moveBy(delta_x, -1 * delta_y);             
    }

    QTransform trans;
    trans.rotate(rot);

    // m_zoom_init = 1um / height;
    // trans.scale = 1 um / height / pos->zoom = 1 um / height / (1 um / now_height) = now_height / height

    trans.scale((*m_zoom_init)/pos->zoom, ((*m_zoom_init)/pos->zoom) * (pos->tilt / 90));
      //zoom 초기가 25%면 ..
    setTransform(trans);
}

void SuperItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "x : " << this->x() << " y : " << (-1 * this->y());
    qDebug() << "x : " << convertPosX() << " y : " << convertPosY();
}

void SuperItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::red);
    painter->drawRoundedRect(-60,-60,120,120,5,5);
    painter->setPen(Qt::blue);
    painter->drawLine(-60,-60,60,-60);
}

QRectF SuperItem::boundingRect() const{ // 나중에 이해를 위한 도움 요청 필요
    return QRectF(-61,-61,122,122);
}

double SuperItem::convertPosX()
{
    return (this->x() - (*m_from_window_to_box_x))/(*m_scale)+(*m_min_x); // 원점이 (0,0) 이 아닐 경우 skew(m_min_x) 추가
}

double SuperItem::convertPosY()
{
    return -1 * (this->y() - (*m_from_window_to_box_y))/(*m_scale)+(*m_min_y);
}

FormMap::FormMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormMap)
{
    ui->setupUi(this);

}

FormMap::~FormMap()
{
    delete ui;
}

void FormMap::receiveFile(T2D &t2d)
{
    QGraphicsRectItem *rectItem = new QGraphicsRectItem;
    QGraphicsScene *scene = new QGraphicsScene(this);
    super = new SuperItem();
    super->getScaleValue(&m_scale, &m_from_window_to_box_x, &m_from_window_to_box_y, &m_min_x, &m_min_y, &m_zoom_init);

    m_min_x = t2d.LayoutMinMax.minx;
    m_min_y = t2d.LayoutMinMax.miny;
    double width = t2d.LayoutMinMax.maxx - t2d.LayoutMinMax.minx;
    double height = t2d.LayoutMinMax.maxy - t2d.LayoutMinMax.miny;
    m_zoom_init = 1 / height;
    m_scale = m_gray_box_size/qMax(width,height);    //가장 긴 곳을 120으로 맞춤
    double box_x_zero_point = -1 * m_scale * width /2;
    double box_y_zero_point = -1 * m_scale * height /2;
    m_box_width = m_scale * width;
    m_box_height = m_scale * height;
    m_window_width = ui->graphicsView->size().width();
    m_window_height = ui->graphicsView->size().height();
    m_from_window_to_box_x = m_window_width/2 - m_box_width/2;
    m_from_window_to_box_y = -1 * (m_window_height/2 - m_box_height/2);

    rectItem->setRect(box_x_zero_point,box_y_zero_point,m_box_width,m_box_height);
    rectItem->setBrush(QBrush(QColor(Qt::gray)));
    rectItem->setPos(m_window_width/2,-m_window_height/2); //좌측 위 (0,0) 에서 부터 중앙으로 이동
    scene->setSceneRect(m_from_window_to_box_x,(m_from_window_to_box_y - m_box_height),m_box_width,m_box_height); //Y 좌표는 위에서 얼만큼 내려오는가 라서
    scene->addItem(rectItem);           //사각형
    scene->addItem(super);    

    QObject::connect(this,&FormMap::signalMove,super,&SuperItem::slotMove);

    ui->graphicsView->setScene(scene);

}

void FormMap::receivePointPos(POS_MONITORING &pos)
{
    this->pos = &pos;
}

void FormMap::changePos()
{    
    emit signalMove(pos);
}
