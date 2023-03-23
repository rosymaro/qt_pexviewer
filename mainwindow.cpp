#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Rendering/Src/dtaorendersystem.h"
#include "Rendering/Src/lve_model.hpp"


#include <QMouseEvent>
#include <QKeyEvent>
#include <QDockWidget>
#include <QGraphicsRectItem>
#include <QGraphicsOpacityEffect>
#include <QFile>
#include <QFileDialog>
#include <QString>

#include <fstream>
#include <iostream>
#include <string>
#include <QIODevice>
#include <QTextStream>

MainWindow::MainWindow(LveWindow *w)
    : m_window(w)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *wrapper = QWidget::createWindowContainer(w);
    ui->graphicsView->setViewport(wrapper);

    QDockWidget *dockHier = new QDockWidget(tr("Hierarchy"), this);
    dockHier->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockHier->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, dockHier);
    formHier = new FormHier;
    dockHier->setWidget(formHier);

    QDockWidget *dockLayer = new QDockWidget(tr("Layer Information"), this);
    dockLayer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockLayer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, dockLayer);
    formLayer = new FormLayer(this->m_window);
    dockLayer->setWidget(formLayer);

    QDockWidget *dockMap = new QDockWidget(tr("Map"), this);
    dockMap->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockMap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::LeftDockWidgetArea, dockMap);
    formMap = new FormMap;
    dockMap->setWidget(formMap);

    QDockWidget *dockTop = new QDockWidget(tr("Topview"), this);
    dockTop->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockTop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::LeftDockWidgetArea, dockTop);
    formTop = new FormTop;
    dockTop->setWidget(formTop);

    QDockWidget *dockInfo = new QDockWidget(tr("Info"), this);
    dockInfo->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::LeftDockWidgetArea, dockInfo);
    formInfo = new FormInfo;
    dockInfo->setWidget(formInfo);

    resizeDocks({dockInfo, dockTop, dockMap, dockLayer, dockHier}, {200,200}, Qt::Horizontal);
    resizeDocks({dockTop, dockMap}, {200,200}, Qt::Vertical);

    input_dataS = new all_data;

    // test //
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    pos.tilt = 90;
    pos.rotation = 0;
    pos.zoom = 1;
    pos.window_zoom = 1;
    formInfo->slotPos(pos);
    formTop->slotPos(pos);


    /// connect ////////////////////
    QObject::connect(ui->actionOpen_file, SIGNAL(triggered()), this, SLOT(on_actionOpen_file_triggered));
    QObject::connect(this, SIGNAL(sendSelectFileName(QString)), input_dataS, SLOT(receiveSelectFileName(QString)));
    QObject::connect(input_dataS, SIGNAL(sendSplitData(int, int, const QVector <QVector <QString>> &)), formHier, SLOT(ReceiveSplitData(int, int, const QVector <QVector <QString>> &)));
    QObject::connect(input_dataS, SIGNAL(sendSplitData(int, int, const QVector <QVector <QString>> &)), formLayer, SLOT(ReceiveSplitData(int, int, const QVector <QVector <QString>> &)));
    QObject::connect(formLayer, SIGNAL(outputLayerStatus(QString)), this, SLOT(inputLayerStatus(QString)));
    QObject::connect(formInfo, SIGNAL(signalPos()), this, SLOT(slotPos()));
}

void MainWindow::shareGeo(QRect size)
{
    QRect windowSize = size;
    qDebug() << "shareGeo : " << windowSize;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotInfoText(QString funcName, POS_MONITORING value)
{
    formInfo->slotInfoText(funcName,value);
    formMap->slotInfoText(funcName,value);
    //qDebug() << "slotInfoText" << funcName << value.x << " "<< value.y << " "<< value.z << " ";
///// temp //////
    QString text = funcName + " : " ;
    for (int i = text.size() ; i < 30 ; i++)
    {
        text.append(" ");
    }

    statusText.prepend(text);

    if (statusText.size() > 120)
        statusText.remove(120,statusText.size()-120);
    ui->statusbar->showMessage(statusText);

}

void MainWindow::on_actionOpen_file_triggered()
{

    QString file_name = QFileDialog::getOpenFileName(this, "ì¼  í",".","Files(*.*)");
    //qDebug() << file_name;

    emit sendSelectFileName(file_name);

}

void MainWindow::inputLayerStatus(QString text)
{
    ui->statusbar->showMessage(text);
    formTop->drawingClear();
    formTop->drawing();
}

void MainWindow::inputPosInformation()
{

    //execute (pos)
}

void MainWindow::on_actionOpen_Layout_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "ì¼  í",".","Files(*.*)");
    DtaoRenderSystem * renderer = this->m_window->getRenderer();

    //renderer->createNewObject(MODEL_TYPE::MODEL_TYPE_LAYOUT, file_name.toStdString());


}

void MainWindow::on_actionOpen_DB_triggered(){
    std::cout << "callv" << std::endl;
    QString file_name = QFileDialog::getOpenFileName(this, "OpenDB", ".", "Text (*.txt) ;; Files (*.*)");
    if(file_name != ""){
        std::cout << file_name.toStdString() << std::endl;
        this->t2d.text2data(file_name.toStdString());

//        printf("%-9f %-9f %-9f %-9f %-9f %-9f", t2d.LayoutMinMax.minx, t2d.LayoutMinMax.miny, t2d.LayoutMinMax.maxx, t2d.LayoutMinMax.maxy, t2d.LayoutMinMax.minz, t2d.LayoutMinMax.maxz);
//        for(int i = 0 ; i < t2d.LayoutData10by10.size() ; i++){
//            printf("\n%-9s %-9d %-9d %-9d %-9d %-9d %-9d %-9f %-9f ",
//                   t2d.LayoutData10by10[i].layername.c_str(),
//                   t2d.LayoutData10by10[i].layernum,
//                   t2d.LayoutData10by10[i].datatype,
//                   t2d.LayoutData10by10[i].color.r,
//                   t2d.LayoutData10by10[i].color.g,
//                   t2d.LayoutData10by10[i].color.b,
//                   t2d.LayoutData10by10[i].color.a,
//                   t2d.LayoutData10by10[i].bot,
//                   t2d.LayoutData10by10[i].top);
//            for(int j = 0 ; j < (t2d.LayoutData10by10[i].xy).size() ; j++){
//                for(int x = 0 ; x < (t2d.LayoutData10by10[i].xy[j]).size() ; x++){
//                    printf("\n%d %d %d", j, x, (t2d.LayoutData10by10[i].xy[j][x]).size());
//                    for(int y = 0 ; y < (t2d.LayoutData10by10[i].xy[j][x]).size() ; y++){
//                        printf("\n%-9f %-9f %-9f %-9f",
//                               t2d.LayoutData10by10[i].xy[j][x][y].minx,
//                               t2d.LayoutData10by10[i].xy[j][x][y].miny,
//                               t2d.LayoutData10by10[i].xy[j][x][y].maxx,
//                               t2d.LayoutData10by10[i].xy[j][x][y].maxy);
//                    }
//                }
//            }
//        }
        //to map data



        pos.x = t2d.LayoutMinMax.minx;
        pos.y = t2d.LayoutMinMax.miny;
        pos.z = 0;
        pos.tilt = 90;
        pos.rotation = 0;
        pos.zoom = 1;
        formInfo->outputText();

        double zoomScale = std::max(t2d.LayoutMinMax.maxx-t2d.LayoutMinMax.minx ,t2d.LayoutMinMax.maxy-t2d.LayoutMinMax.miny)/10;
        formMap->receiveSize_t2d(t2d.LayoutMinMax.minx,t2d.LayoutMinMax.miny,t2d.LayoutMinMax.maxx,t2d.LayoutMinMax.maxy,zoomScale);

        //to top data


        DtaoRenderSystem * renderer = this->m_window->getRenderer();

        renderer->createT2DObject(MODEL_TYPE::MODEL_TYPE_LAYOUT, t2d);

        formInfo->receiveFile(t2d);
        formTop->receiveFile(t2d);
        formLayer->ReceiveLayerInformation(t2d);
        //test



    }
    fflush(stdout);
    cout << "test end" << endl;

}

void MainWindow::slotPos()
{
    formTop->changePos();
}












void MainWindow::on_actionTop_triggered()
{
    qDebug() << "go?";
    //cocococo
}
