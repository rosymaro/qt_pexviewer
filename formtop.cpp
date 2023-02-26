#include "formtop.h"
#include "ui_formtop.h"
#include <QGraphicsItem>
#include <QScrollBar>
#include <QVector>
#include <QMap>
#include <iostream>
#include <QDebug>

FormTop::FormTop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormTop)
{
    ui->setupUi(this);
}

void FormTop::receiveFile(QVector<QStringList> strVector)
{
//    QString temp;
//    temp = strVector[1][3];
//    float fTemp = temp.toFloat()*1000000;
//    int iTemp = int(fTemp);
    float minX=0, maxX=0, minY=0, maxY=0;

    QMap<int, QColor> mapColor;
    mapColor.insert(15,Qt::red);
    mapColor.insert(16,Qt::yellow);
    mapColor.insert(17,Qt::green);
    mapColor.insert(18,Qt::blue);
    mapColor.insert(19,Qt::magenta);
    mapColor.insert(714,Qt::red);
    mapColor.insert(177,Qt::red);

    qDebug() << "mapColor" <<mapColor;

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(QColor(Qt::white)));
    //scene->setSceneRect(0,0,200,200);
    QPen mPen;
    mPen.setWidth(5);
    mPen.setColor(Qt::red);
// 기존에 있는 vector 를 남겨놔야 함 : delete 를 위해
    //            for(auto cur_item : rectItemList){
    //                delete cur_item;
    //            }
    //            rectItemList.clear();


    QVector<QGraphicsRectItem*> rectItemList;

    QGraphicsLineItem *zeroX = new QGraphicsLineItem;
    QGraphicsLineItem *zeroY = new QGraphicsLineItem;
    QTransform trans;
    trans.scale(1,-1);

    for (auto &data : strVector)
    {

        if (data.size() == 8)
        {
            QGraphicsRectItem *rectItem = new QGraphicsRectItem;
            rectItemList.push_back(rectItem);


            float x = 500*(data[2].toFloat());
            float y = 500*(data[3].toFloat());
            float w = 500*(data[4].toFloat() - data[2].toFloat());
            float h = 500*(data[5].toFloat() - data[3].toFloat());

            if (x<minX){minX = x;}
            if (x>maxX){maxX = x;}
            if (y<minY){minY = y;}
            if (y>maxY){maxY = y;}
            rectItem->setRect(x,y,w,h);
            rectItem->setBrush(QBrush(QColor(mapColor[data[0].toInt()])));
            rectItem->setOpacity(0.5);
            rectItem->setZValue(data[6].toFloat());

            rectItem->setTransform(trans);
            scene->addItem(rectItem);
        }
    }
    zeroX->setLine(0,0,maxX,0);
    zeroY->setLine(0,0,0,maxY);
    zeroX->setPen(QPen(QColor(Qt::red)));
    zeroY->setPen(QPen(QColor(Qt::red)));
    zeroX->setZValue(400);
    zeroY->setZValue(400);
    zeroY->setTransform(trans);
    scene->addItem(zeroX);
    scene->addItem(zeroY);
    ui->graphicsView->setScene(scene);
//    float dx = 100-(maxX-minX)/2;
//    float dy = 100-(maxY-minY)/2;



//    ui->graphicsView->horizontalScrollBar()->setValue(ui->graphicsView->horizontalScrollBar()->value()+dx);
//    ui->graphicsView->verticalScrollBar()->setValue(ui->graphicsView->verticalScrollBar()->value()+dy);
//    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


}




FormTop::~FormTop()
{
    delete ui;
}
