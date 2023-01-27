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
    const float amount = e->angleDelta().y() / 8;
//    m_renderer->moveZoom(amount);
    QString funcValue = "moveZoom : " + QString::number(amount);
    emit outputStatus(funcValue);
    qDebug()<<funcValue;
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
//            m_renderer->renderRotateX(dx / 10.0f);
            QString funcValue = "renderRotateX : " + QString::number(dx / 10.0f);
            emit outputStatus(funcValue);
            qDebug()<<funcValue;
        }
        else if (m_mouseButton == 4)
        {
//            m_renderer->renderMoveX(dx / 10.0f);
            QString funcValue = "renderMoveX : " + QString::number(dx / 10.0f);
            emit outputStatus(funcValue);
            qDebug()<<funcValue;
        }
    }

    if (dy)
    {
        if (m_mouseButton == 2)
        {
//            m_renderer->renderRotateY(dy / 10.0f);
            QString funcValue = "renderRotateY : " + QString::number(dy / 10.0f);
            emit outputStatus(funcValue);
            qDebug()<<funcValue;
        }
        else if (m_mouseButton == 4)
        {
//            m_renderer->renderMoveY(dy / 10.0f);
            QString funcValue = "renderMoveY : " + QString::number(dy / 10.0f);
            emit outputStatus(funcValue);
            qDebug()<<funcValue;
        }
    }

}

