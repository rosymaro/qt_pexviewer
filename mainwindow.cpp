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

MainWindow::MainWindow(LveWindow* w)
    : m_window(w)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* wrapper = QWidget::createWindowContainer(w);
    ui->graphicsView->setViewport(wrapper);

    dockHier = new QDockWidget(tr("Hierarchy"), this);
    dockHier->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockHier->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dockHier->setObjectName("DockWidgetHier");
    addDockWidget(Qt::RightDockWidgetArea, dockHier);
    formHier = new FormHier;
    dockHier->setWidget(formHier);

    dockLayer = new QDockWidget(tr("Layer Information"), this);
    dockLayer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockLayer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dockLayer->setObjectName("DockWidgetLayer");
    addDockWidget(Qt::RightDockWidgetArea, dockLayer);
    formLayer = new FormLayer(this->m_window);
    dockLayer->setWidget(formLayer);

    dockMap = new QDockWidget(tr("Map"), this);
    dockMap->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockMap->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dockMap->setObjectName("DockWidgetMap");
    addDockWidget(Qt::LeftDockWidgetArea, dockMap);
    formMap = new FormMap;
    dockMap->setWidget(formMap);

    dockTop = new QDockWidget(tr("Topview"), this);
    dockTop->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockTop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dockTop->setObjectName("DockWidgetTop");
    addDockWidget(Qt::LeftDockWidgetArea, dockTop);
    formTop = new FormTop;
    dockTop->setWidget(formTop);

    dockInfo = new QDockWidget(tr("Info"), this);
    dockInfo->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dockInfo->setObjectName("DockWidgetInfo");
    addDockWidget(Qt::LeftDockWidgetArea, dockInfo);
    formInfo = new FormInfo;
    dockInfo->setWidget(formInfo);

    resizeDocks({ dockInfo, dockTop, dockMap, dockLayer, dockHier }, { 200,200 }, Qt::Horizontal);
    resizeDocks({ dockTop, dockMap }, { 200,200 }, Qt::Vertical);
    resizeDocks({ dockLayer, dockHier }, { 350,300 }, Qt::Vertical);

    input_dataS = new all_data;

    // test //
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    pos.tilt = 90;
    pos.rotation = 0;
    pos.zoom = 1;
    pos.window_zoom = 1;
    formMap->receivePointPos(pos);
    formTop->receivePointPos(pos);
    formInfo->receivePointPos(pos);

    /// connect ////////////////////
    QObject::connect(ui->actionOpen_file, SIGNAL(triggered()), this, SLOT(on_actionOpen_file_triggered));
    QObject::connect(ui->actionOpen_PEX, SIGNAL(triggered()), this, SLOT(on_actionOpen_PEX_triggered));
//    QObject::connect(this, SIGNAL(sendSelectFileName(QString)), input_dataS, SLOT(receiveSelectFileName(QString)));
//    QObject::connect(input_dataS, SIGNAL(sendSplitData(int, int, const QVector <QVector <QString>> &)), formHier, SLOT(ReceiveSplitData(int, int, const QVector <QVector <QString>> &)));
//    QObject::connect(input_dataS, SIGNAL(sendSplitData(int, int, const QVector <QVector <QString>> &)), formLayer, SLOT(ReceiveSplitData(int, int, const QVector <QVector <QString>> &)));
//    QObject::connect(formLayer, SIGNAL(outputLayerStatus(QString)), this, SLOT(inputLayerStatus(QString)));
    //QObject::connect(formTop, SIGNAL(signalChangePos(QString, POS_MONITORING)), this, SLOT(slotInfoText(QString, POS_MONITORING)));
    QObject::connect(formInfo, SIGNAL(signalDirectlyInputPos()), this, SLOT(slotDirectlyInputPos()));
    QObject::connect(formInfo, SIGNAL(signalStatusBarMsg(QString)), this, SLOT(slotStatusBarMsg(QString)));
    QObject::connect(dockMap, SIGNAL(visibilityChanged(bool)), this, SLOT(on_docker_Map_stateChanged(bool)));
    QObject::connect(dockTop, SIGNAL(visibilityChanged(bool)), this, SLOT(on_docker_TopView_stateChanged(bool)));
    QObject::connect(dockInfo, SIGNAL(visibilityChanged(bool)), this, SLOT(on_docker_Info_stateChanged(bool)));
    QObject::connect(dockHier, SIGNAL(visibilityChanged(bool)), this, SLOT(on_docker_Hier_stateChanged(bool)));
    QObject::connect(dockLayer, SIGNAL(visibilityChanged(bool)), this, SLOT(on_docker_Layer_stateChanged(bool)));
    // 평가용
    dev_dialog = new DevDialog(this);
    devDialog();
}

void MainWindow::shareGeo(QRect size)
{
    QRect windowSize = size;
    qDebug() << "shareGeo : " << windowSize;
}

void MainWindow::devDialog()
{
    dev_dialog->sendParameter(&is_pos_checked, &pos);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotInfoText(QString funcName, POS_MONITORING value)
{

    if (value.x != pos_past.x ||
        value.y != pos_past.y ||
        value.z != pos_past.z ||
        (value.tilt - pos_past.tilt) > 0.01 ||
        (value.rotation - pos_past.rotation) > 0.01 ||
        value.zoom != pos_past.zoom)
    {
        pos.x = value.x;
        pos.y = value.y;
        pos.z = value.z;
        pos.tilt = value.tilt;
        pos.rotation = value.rotation;
        pos.zoom = value.zoom;

        sendPositionDataToDocker(); // changePos() refactoring

        pos_past.x = pos.x;
        pos_past.y = pos.y;
        pos_past.z = pos.z;
        pos_past.tilt = pos.tilt;
        pos_past.rotation = pos.rotation;
        pos_past.zoom = pos.zoom;
    }


    //qDebug() << "slotInfoText" << funcName << value.x << " "<< value.y << " "<< value.z << " ";
///// temp //////
//    QString text = funcName + " : " ;
//    for (int i = text.size() ; i < 30 ; i++)
//    {
//        text.append(" ");
//    }

//    statusText.prepend(text);

//    if (statusText.size() > 120)
//        statusText.remove(120,statusText.size()-120);
//    ui->statusbar->showMessage(statusText);

}

void MainWindow::on_actionOpen_file_triggered()
{

    QString file_name = QFileDialog::getOpenFileName(this, "파일 선택", ".", "Files(*.*)");
    //qDebug() << file_name;

    emit sendSelectFileName(file_name);

}

void MainWindow::on_actionOpen_PEX_triggered()
{
    if (this->t2d.LayoutData10by10.empty() == false) {
        QString file_name = QFileDialog::getOpenFileName(this, "파일 선택", ".", "Files(*.*)");

        DtaoRenderSystem* renderer = this->m_window->getRenderer();

        renderer->createNewPEXResObject(file_name);
        renderer->createNewPEXCapObject(file_name,this->t2d);

        //test
    }
    else {
        qDebug() << "Layout Load 후 PEX를 불러올 수 있습니다";
    }
}

void MainWindow::inputLayerStatus(QString text)
{
    if(formLayer->m_layer_all_check == false){
        ui->statusbar->showMessage(text);
        /*formTop->drawingClear();
        formTop->drawing();*/
        sendPositionDataToDocker();
    }
}

void MainWindow::inputPosInformation()
{

    //execute (pos)
}

void MainWindow::on_actionOpen_Layout_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "파일 선택", ".", "Files(*.*)");
    DtaoRenderSystem* renderer = this->m_window->getRenderer();

    //renderer->createNewObject(MODEL_TYPE::MODEL_TYPE_LAYOUT, file_name.toStdString());


}

void MainWindow::on_actionOpen_DB_triggered() {
    std::cout << "callv" << std::endl;
    QString file_name = QFileDialog::getOpenFileName(this, "OpenDB", ".", "Text (*.txt) ;; Files (*.*)");
    if (file_name != "") {
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
        //        for(int i = 0 ; i < t2d.HierarchyInstance.size() ; i++){
        //            printf("\n%-3d %-19s %-9s %-9f %-9f %-9f %-9f",
        //                                t2d.HierarchyInstance[i].level,
        //                                t2d.HierarchyInstance[i].name.c_str(),
        //                                t2d.HierarchyInstance[i].num.c_str(),
        //                                t2d.HierarchyInstance[i].box.minx,
        //                                t2d.HierarchyInstance[i].box.miny,
        //                                t2d.HierarchyInstance[i].box.maxx,
        //                                t2d.HierarchyInstance[i].box.maxy);
        //        }

        //formHier->createHierarchyTree(&t2d);

        pos.x = t2d.LayoutMinMax.minx;
        pos.y = t2d.LayoutMinMax.miny;
        pos.z = 0;
        pos.tilt = 90;
        pos.rotation = 0;
        pos.zoom = 1;
        formInfo->setTextPosTotal(&pos);

        //to top data


        DtaoRenderSystem* renderer = this->m_window->getRenderer();

        renderer->createT2DObject(MODEL_TYPE::MODEL_TYPE_LAYOUT, t2d);

        //cout << "!!" << renderer->getRenderScale() << endl;
        formInfo->receiveFile(t2d);
        formTop->receiveFile(t2d, 100, 0.1);
        formMap->receiveFile(t2d);
        formLayer->ReceiveLayerInformation(t2d);
        //test



    }
    fflush(stdout);
    cout << "test end" << endl;

}

void MainWindow::slotDirectlyInputPos() //point info 에서 직접 입력한 경우
{
    sendPositionDataToDocker();
    DtaoRenderSystem * renderer = this->m_window->getRenderer();
    renderer->translateLayerPosition(pos);
    //요청사항 230324 vulkan 에서도 수정을 여기서 실행
}

void MainWindow::slotStatusBarMsg(QString msg)
{
    ui->statusbar->showMessage(msg);
}



void MainWindow::on_actionDemo_triggered()
{
    qDebug() << "go?";
    DtaoRenderSystem* renderer = this->m_window->getRenderer();
    renderer->cameraController.triggerDemo();
}




void MainWindow::on_actionTop_triggered()
{
    qDebug() << "go?";
    //cocococo
}

void MainWindow::on_docker_Map_triggered()
{
    if (dockMap->isHidden()) {
//        MainWindow::restoreGeometry(dock_widget_geometry);
//        MainWindow::restoreState(dock_widget_state);
        dockMap->show();
    }
    else {
//        dock_widget_geometry = MainWindow::saveGeometry();
//        dock_widget_state = MainWindow::saveState();
        dockMap->hide();
    }
}

void MainWindow::on_docker_Map_stateChanged(bool visibility)
{
    if (visibility) {
        ui->docker_Map->setCheckable(true);
        ui->docker_Map->setChecked(true);
    } else {
        ui->docker_Map->setCheckable(false);
        ui->docker_Map->setChecked(false);
    }
}

void MainWindow::on_docker_TopView_triggered()
{

    if (dockTop->isHidden()) {
//        MainWindow::restoreGeometry(dock_widget_geometry);
//        MainWindow::restoreState(dock_widget_state);
        dockTop->show();

    }
    else {
//        dock_widget_geometry = MainWindow::saveGeometry();
//        dock_widget_state = MainWindow::saveState();
        dockTop->hide();
    }
}

void MainWindow::on_docker_TopView_stateChanged(bool visibility)
{
    if (visibility) {
        ui->docker_TopView->setCheckable(true);
        ui->docker_TopView->setChecked(true);
    } else {
        ui->docker_TopView->setCheckable(false);
        ui->docker_TopView->setChecked(false);
    }
}

void MainWindow::on_docker_Info_triggered()
{
    if (dockInfo->isHidden()) {
//        MainWindow::restoreGeometry(dock_widget_geometry);
//        MainWindow::restoreState(dock_widget_state);
        dockInfo->show();

    }
    else {
//        dock_widget_geometry = MainWindow::saveGeometry();
//        dock_widget_state = MainWindow::saveState();
        dockInfo->hide();
    }
}

void MainWindow::on_docker_Info_stateChanged(bool visibility)
{
    if (visibility) {
        ui->docker_Info->setCheckable(true);
        ui->docker_Info->setChecked(true);
    } else {
        ui->docker_Info->setCheckable(false);
        ui->docker_Info->setChecked(false);
    }
}

void MainWindow::on_docker_Hier_triggered()
{
    if (dockHier->isHidden()) {
//        MainWindow::restoreGeometry(dock_widget_geometry);
//        MainWindow::restoreState(dock_widget_state);
        dockHier->show();

    }
    else {
//        dock_widget_geometry = MainWindow::saveGeometry();
//        dock_widget_state = MainWindow::saveState();
        dockHier->hide();
    }
}

void MainWindow::on_docker_Hier_stateChanged(bool visibility)
{
    if (visibility) {
        ui->docker_Hier->setCheckable(true);
        ui->docker_Hier->setChecked(true);
    } else {
        ui->docker_Hier->setCheckable(false);
        ui->docker_Hier->setChecked(false);
    }
}

void MainWindow::on_docker_Layer_triggered()
{
    if (dockLayer->isHidden()) {
//        MainWindow::restoreGeometry(dock_widget_geometry);
//        MainWindow::restoreState(dock_widget_state);
        dockLayer->show();

    }
    else {
//        dock_widget_geometry = MainWindow::saveGeometry();
//        dock_widget_state = MainWindow::saveState();
        dockLayer->hide();
    }
}

void MainWindow::on_docker_Layer_stateChanged(bool visibility)
{
    if (visibility) {
        ui->docker_Layer->setCheckable(true);
        ui->docker_Layer->setChecked(true);
    } else {
        ui->docker_Layer->setCheckable(false);
        ui->docker_Layer->setChecked(false);
    }
}

void MainWindow::on_toggle_top_triggered()
{
    DtaoRenderSystem* renderer = this->m_window->getRenderer();
    renderer->cameraController.camera_moving_flag.toggleDemension = true;
    renderer->cameraController.camera_moving_flag.toggleDemensionXY = true;
}

void MainWindow::on_PEX_window_triggered()
{
    pexdialog = new PexDialog(this);
    pexdialog->show();
    pexdialog->raise();
    pexdialog->activateWindow();

}

void MainWindow::sendPositionDataToDocker()
{
//dev_dialog
    if (is_pos_checked)
    {
        formMap->changePos();
        formTop->changePos();
        formInfo->changePos();
        dev_dialog->changePos();
    }
}

void MainWindow::on_actionDialog_on_off_triggered()
{
    dev_dialog->show();
    dev_dialog->raise();
    dev_dialog->activateWindow();
}

void MainWindow::on_actionToyCAD_triggered()
{
    DtaoRenderSystem* renderer = this->m_window->getRenderer();
    renderer->createToyCADObjects("ToyCAD/Data/test.csv");

}
