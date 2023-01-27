#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>




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
    ui->statusbar->showMessage(text);
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
