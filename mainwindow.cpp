#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>


MainWindow::MainWindow(VulkanWindow *w)
    : m_window(w)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *wrapper = QWidget::createWindowContainer(w);
    ui->verticalLayout_2->addWidget(wrapper);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::inputStatus(QString text)
{
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
        QString funcValue = "windowZoom : " + QString::number(amount);
        emit outputStatus(funcValue);
        qDebug()<<funcValue;
    }
    else
    {
//      m_renderer->moveZoom(amount);
        QString funcValue = "moveZoom : " + QString::number(amount);
        emit outputStatus(funcValue);
        qDebug()<<funcValue;
    }
}

void VulkanWindow::mousePressEvent(QMouseEvent *e)
{
    m_mouseButton = e->buttons();
    m_lastPos = e->pos();
    qDebug() << m_mouseButton;
    qDebug() << m_lastPos;
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_mouseButton = 0;
    qDebug() << m_mouseButton;
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
            QString funcValue = "rotateRenderX : " + QString::number(dx / 10.0f);
            emit outputStatus(funcValue);
            qDebug()<<funcValue;
        }
        else if (m_mouseButton == 4)
        {
//            m_renderer->moveRenderX(dx / 10.0f);
            QString funcValue = "moveRenderX : " + QString::number(dx / 10.0f);
            emit outputStatus(funcValue);
            qDebug()<<funcValue;
        }
    }

    if (dy)
    {
        if (m_mouseButton == 2)
        {
//            m_renderer->rotateRenderY(dy / 10.0f);
            QString funcValue = "rotateRenderY : " + QString::number(dy / 10.0f);
            emit outputStatus(funcValue);
            qDebug()<<funcValue;
        }
        else if (m_mouseButton == 4)
        {
//            m_renderer->moveRenderY(dy / 10.0f);
            QString funcValue = "moveRenderY : " + QString::number(dy / 10.0f);
            emit outputStatus(funcValue);
            qDebug()<<funcValue;
        }
    }
}

void VulkanWindow::keyPressEvent(QKeyEvent *e)
{
    const float amount = e->modifiers().testFlag(Qt::ShiftModifier) ? 1.0f : 0.1f;
    QString funcValue;
    switch (e->key()) {
    case Qt::Key_Up:
//        m_renderer->moveGdsY(amount);
        funcValue = "moveGdsY : " + QString::number(amount);
        emit outputStatus(funcValue);
        qDebug()<<funcValue;
        break;
    case Qt::Key_Down:
//        m_renderer->moveGdsY(-amount);
        funcValue = "moveGdsY : " + QString::number(-amount);
        emit outputStatus(funcValue);
        qDebug()<<funcValue;
        break;
    case Qt::Key_Right:
//        m_renderer->moveGdsX(-amount);
        funcValue = "moveGdsX : " + QString::number(amount);
        emit outputStatus(funcValue);
        qDebug()<<funcValue;
        break;
    case Qt::Key_Left:
//        m_renderer->moveGdsX(amount);
        funcValue = "moveGdsX : " + QString::number(-amount);
        emit outputStatus(funcValue);
        qDebug()<<funcValue;
        break;
    case Qt::Key_PageUp:
//        m_renderer->moveGdsZ(amount);
        funcValue = "moveGdsZ : " + QString::number(amount);
        emit outputStatus(funcValue);
        qDebug()<<funcValue;
        break;
    case Qt::Key_PageDown:
//        m_renderer->moveGdsZ(amount);
        funcValue = "moveGdsZ : " + QString::number(-amount);
        emit outputStatus(funcValue);
        qDebug()<<funcValue;
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
