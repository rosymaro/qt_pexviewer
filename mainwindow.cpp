#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "forminfo.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QDockWidget>
#include <QRect>

//QSetting 은 환경 저장 관련

MainWindow::MainWindow(VulkanWindow *w)
    : m_window(w)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *wrapper = QWidget::createWindowContainer(w);
    ui->verticalLayout_2->addWidget(wrapper);

    QDockWidget *dockHier = new QDockWidget(tr("Hierarchy"), this);
    QDockWidget *dockLayer = new QDockWidget(tr("Layer Information"), this);
    QDockWidget *dockTop = new QDockWidget(tr("GDS View"), this);
    QDockWidget *dockInfo = new QDockWidget(tr("Camera Information"), this);
    QDockWidget *dockMap = new QDockWidget(tr("Map"), this);

    dockHier->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockLayer->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockTop->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockInfo->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockMap->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    addDockWidget(Qt::LeftDockWidgetArea, dockHier);
    addDockWidget(Qt::LeftDockWidgetArea, dockLayer);
    addDockWidget(Qt::RightDockWidgetArea, dockTop);
    addDockWidget(Qt::RightDockWidgetArea, dockInfo);
    addDockWidget(Qt::RightDockWidgetArea, dockMap);

    formHier = new FormHier;
    formLayer = new FormLayer;
    formTop = new FormTop;
    formInfo = new FormInfo;
    formMap = new FormMap;

    dockHier->setWidget(formHier);
    dockLayer->setWidget(formLayer);
    dockTop->setWidget(formTop);
    dockInfo->setWidget(formInfo);
    dockMap->setWidget(formMap);

    dockHier->setFloating(true);
    dockLayer->setFloating(true);
    dockInfo->setFloating(true);
    dockTop->setFloating(true);
    dockMap->setFloating(true);

//    int leftMain = this->geometry().x();
//    int topMain = this->geometry().y();
//    int heightMain = this->geometry().height();
//    int widthMain = this->geometry().width();

    dockTop->setGeometry(leftMain,topMain,200,100);
    dockHier->setGeometry(leftMain+200,topMain+100,200,100);
    dockInfo->setGeometry(leftMain+400,topMain+200,200,100);
    dockLayer->setGeometry(leftMain+600,topMain+300,200,100);
    dockMap->setGeometry(leftMain+800,topMain+400,200,100);


    // :::: study ::::
    // formInfo 에 signal/slot 으로 넘기려고 하였으나
    // 여기에서 선언을 할 경우 직접 함수 호출 가능함
    // connect 에서 this 이면 가능한 것으로 보임
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Vulkan window 에서 보내는 Signal 을 받기 위한 함수
void MainWindow::slotInfoText(QString funcName, float value)
{
    formInfo->slotInfoText(funcName,value);
    qDebug() << "slotInfoText" << value;
///// temp //////
    QString text = funcName + " : " + QString::number(value);
    for (int i = text.size() ; i < 30 ; i++)
    {
        text.append(" ");
    }

    statusText.prepend(text);

    if (statusText.size() > 120)
        statusText.remove(120,statusText.size()-120);
    ui->statusbar->showMessage(statusText);

}

void MainWindow::moveEvent(QMoveEvent *)
{
    qDebug()<< "moveEvent" << this->geometry() << start;
    leftMain = this->geometry().x();
    topMain = this->geometry().y();
    heightMain = this->geometry().height();
    widthMain = this->geometry().width();




}

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
    m_renderer = new VulkanRenderer(this);
    return m_renderer;
}

VulkanRenderer::VulkanRenderer(VulkanWindow *w)
    : TriangleRenderer(w)
{
}

void VulkanWindow::wheelEvent(QWheelEvent *e)
{
    //문제가 하나 있는데...
    //프로그램이 선택되어 있지 않아도 moveZoom 명령이 나감
    //근데 Ctrl 은 Vulkan Window 가 선택되었을 때만 됨
    //moveZoom 을 이 Level 까지 끌고 내려와야 함
    const float amount = e->angleDelta().y() / 8;

    if (keyCtrl == true)
    {
//      m_rendere->windowZoom(amount);
        QString funcName = "windowZoom";
        float value = amount;
        emit signalInfoText(funcName, value);
//        qDebug()<<funcName << " : " << value;
    }
    else
    {
//      m_renderer->moveZoom(amount);
        QString funcName = "moveZoom";
        float value = amount;
        emit signalInfoText(funcName, value);
//        qDebug()<<funcName << " : " << value;
    }
}

void VulkanWindow::mousePressEvent(QMouseEvent *e)
{
    m_mouseButton = e->buttons();
    m_lastPos = e->pos();
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_mouseButton = 0;
    QString funcName = "mouseRelease";
    float value = 0;
    emit signalInfoText(funcName, value);
}

void VulkanWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (m_mouseButton == 0)
        return;

    int dx = e->pos().x() - m_lastPos.x();
    int dy = e->pos().y() - m_lastPos.y();

    if (dx)
    {
        if (m_mouseButton == 2)
        {
//            m_renderer->rotateRenderY(dx / 10.0f);
            QString funcName = "rotateRenderX";
            float value = -dx / 10.0f;
            emit signalInfoText(funcName, value);
            qDebug()<< "mouseEvent "<<funcName << " : " << value;
        }
        else if (m_mouseButton == 4)
        {
//            m_renderer->moveRenderX(dx / 10.0f);
            QString funcName = "moveRenderX";
            float value = -dx / 10.0f;
            emit signalInfoText(funcName, value);
//            qDebug()<<funcName << " : " << value;
        }
    }

    if (dy)
    {
        if (m_mouseButton == 2)
        {
//            m_renderer->rotateRenderY(dy / 10.0f);
            QString funcName = "rotateRenderY";
            float value = dy / 10.0f;
            emit signalInfoText(funcName, value);
//            qDebug()<<funcName << " : " << value;
        }
        else if (m_mouseButton == 4)
        {
//            m_renderer->moveRenderY(dy / 10.0f);
            QString funcName = "moveRenderY";
            float value = dy / 10.0f;
            emit signalInfoText(funcName, value);
//            qDebug()<<funcName << " : " << value;
        }
    }
}

void VulkanWindow::keyPressEvent(QKeyEvent *e)
{
    const float amount = e->modifiers().testFlag(Qt::ShiftModifier) ? 1.0f : 0.1f;
    QString funcName;
    float value = 0;
    switch (e->key()) {
    case Qt::Key_Up:
        funcName = "moveGdsY";
        value = amount;
//        m_renderer->moveGdsY(value);
        break;
    case Qt::Key_Down:
        funcName = "moveGdsY";
        value = -amount;
//        m_renderer->moveGdsY(value);
        break;
    case Qt::Key_Right:
        funcName = "moveGdsX";
        value = amount;
//        m_renderer->moveGdsX(value);
        break;
    case Qt::Key_Left:
        funcName = "moveGdsX";
        value = -amount;
//        m_renderer->moveGdsX(value);
        break;
    case Qt::Key_PageUp:
        funcName = "moveGdsZ";
        value = amount;
//        m_renderer->moveGdsZ(value);
        break;
    case Qt::Key_PageDown:
        funcName = "moveGdsZ";
        value = -amount;
//        m_renderer->moveGdsZ(value);
        break;
    case Qt::Key_Control:
        keyCtrl = true;
        qDebug() << "Control";
        break;
    case Qt::Key_Alt:
        keyAlt = true;
        qDebug() << "Alt";
        break;
    case Qt::Key_Shift:
        keyShift = true;
        qDebug() << "Shift";
        break;
    default:
        break;
    }
    emit signalInfoText(funcName, value);
    qDebug()<<funcName << " : " << value;
}
void VulkanWindow::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Control:
        keyCtrl = false;
        qDebug() << "Control off";
        break;
    case Qt::Key_Alt:
        keyAlt = false;
        qDebug() << "Alt off";
        break;
    case Qt::Key_Shift:
        keyShift = false;
        qDebug() << "Shift off";
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{

    if (formHier->isVisible())
    {
        formHier->close();
        formLayer->close();
        formTop->close();
        formInfo->close();
        formMap->close();
    }
    else
    {
        formHier->show();
        formLayer->show();
        formTop->show();
        formInfo->show();
        formMap->show();
    }

}
