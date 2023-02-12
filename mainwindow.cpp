#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "forminfo.h"
#include "formhier.h"

#include <QMouseEvent>
#include <QKeyEvent>
#include <QDockWidget>
#include <QRect>
#include <QTimer>
#include <QGraphicsView>

//QSetting 은 환경 저장 관련

MainWindow::MainWindow(VulkanWindow *w)
    : m_window(w)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *wrapper = QWidget::createWindowContainer(w);
    ui->graphicsView->setViewport(wrapper);

    qDebug()<<"mapToParent : "<<this->mapToParent(this->pos());


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
    formLayer = new FormLayer;
    dockLayer->setWidget(formLayer);

    QDockWidget *dockInfo = new QDockWidget(tr("Info"), this);
    dockInfo->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    addDockWidget(Qt::RightDockWidgetArea, dockInfo);
    formInfo = new FormInfo;
    dockInfo->setWidget(formInfo);



// 230205 하고 싶은 말..
// dockWidget 을 쓰면 floating 상태에서 움직일 수 없음. 근데 이걸 원한다고 함
// dialog 에서는 parent 를 두면 움직일 수 있음. 이 부분을 사용할 수 있을 것 같은데,
// 이러면 더 좋은게 있을 것도 같음


    // :::: study ::::
    // formInfo 에 signal/slot 으로 넘기려고 하였으나
    // 여기에서 선언을 할 경우 직접 함수 호출 가능함
    // connect 에서 this 이면 가능한 것으로 보임
}

void MainWindow::moveEvent(QMoveEvent *)
{
    //실제 Window 의 Size 를 효율적으로 쓴다면 resize 하지 않음
    //move 가 되어야지만 main 에서 mainWindow 호출이 끝나고 값을 가질 수 있음
    qDebug() << "move winS : " << winSize;
    qDebug() << "move winS : " << *winSize;
    qDebug() << "move this : " << this->geometry();

}

MainWindow::~MainWindow()
{
    delete formHier;
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


//    dockInfo->setFloating(true);

//dynamic_cast 를 배워야 할 듯

//    dockInfo->move(100,100);
//    dockInfo->setFloating(true);


}
