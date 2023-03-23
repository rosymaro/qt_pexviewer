
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDockWidget>
#include <QGraphicsRectItem>
#include <QGraphicsOpacityEffect>
#include <QFile>
#include <QFileDialog>
#include <QString>

#include "lve_window.hpp"
#include "dtaorendersystem.h"



LveWindow::LveWindow()
{
}

LveWindow::~LveWindow()
{
    //this->destroy();
}

QVulkanWindowRenderer *LveWindow::createRenderer()
{
    qDebug() << "\n$$$$$ LveWindow::createRenderer()";
    this->m_renderer = new DtaoRenderSystem(this);
    return this->m_renderer;
    //return new DtaoRenderSystem(this);

}

void LveWindow::wheelEvent(QWheelEvent *e)
{
    if( !this->m_renderer->isRenderModelLoaded() ) return;
    const float amount = e->angleDelta().y() / 8;
    QPoint scroll = e->angleDelta();
    this->m_renderer->cameraController.setMouseScroll(scroll);

    if (keyCtrl == true)
    {
        //      m_rendere->windowZoom(amount);
        QString funcName = "windowZoom";
        float value = amount;
        //emit signalInfoText(funcName, value);
        //        qDebug()<<funcName << " : " << value;
    }
    else
    {
        //      m_renderer->moveZoom(amount);
        QString funcName = "moveZoom";
        float value = amount;
        //emit signalInfoText(funcName, value);
        //        qDebug()<<funcName << " : " << value;
    }
}

void LveWindow::mousePressEvent(QMouseEvent *e)
{
    if( !this->m_renderer->isRenderModelLoaded() ) return;
    m_mouseButton = e->buttons();
    Qt::MouseButtons cur_button = e->buttons();
    QString funcName = "mousePress";
    QPoint position = e->pos();
    //emit signalInfoText(funcName, m_lastPos);
    m_lastPos = e->pos();
    this->m_renderer->cameraController.moveButtonPressed(cur_button);
    this->m_renderer->cameraController.setMousePosition(position, 1);

}

void LveWindow::emitStatus(float value){
    QString funcName = "rotateRenderX";
    //emit signalInfoText(funcName, value);
}


void LveWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if( !this->m_renderer->isRenderModelLoaded() ) return;
    m_mouseButton = 0;
    QString funcName = "mouseRelease";
    Qt::MouseButtons cur_button = e->buttons();
    QPoint position = e->pos();
    float value = 0;
    //emit signalInfoText(funcName, cur_button);
    this->m_renderer->cameraController.moveButtonReleased(cur_button);
    this->m_renderer->cameraController.setMousePosition(position, 2);
}

void LveWindow::mouseMoveEvent(QMouseEvent *e)
{
    if( !this->m_renderer->isRenderModelLoaded() ) return;
    QPoint position = e->pos();
    this->m_renderer->cameraController.setMousePosition(position, 0);
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
            //emit signalInfoText(funcName, value);
            //qDebug()<< "mouseEvent "<<funcName << " : " << value;
        }
        else if (m_mouseButton == 4)
        {
            //            m_renderer->moveRenderX(dx / 10.0f);
            QString funcName = "moveRenderX";
            //float value = -dx / 10.0f;
            //emit signalInfoText(funcName, value);
            //            qDebug()<<funcName << " : " << value;
        }
    }

    if (dy)
    {
        if (m_mouseButton == 2)
        {
            //            m_renderer->rotateRenderY(dy / 10.0f);
            QString funcName = "rotateRenderY";
            //float value = dy / 10.0f;
            //emit signalInfoText(funcName, value);
            //            qDebug()<<funcName << " : " << value;
        }
        else if (m_mouseButton == 4)
        {
            //            m_renderer->moveRenderY(dy / 10.0f);
            QString funcName = "moveRenderY";
            //float value = dy / 10.0f;
            //emit signalInfoText(funcName, value);
            //            qDebug()<<funcName << " : " << value;
        }
    }
}

void LveWindow::keyPressEvent(QKeyEvent *e)
{
    if( !this->m_renderer->isRenderModelLoaded() ) return;
    Qt::Key cur_key = (Qt::Key)e->key();
    //QString funcName = "rotateRenderX";
    //float value = 10.0f;
    //emit signalInfoText(funcName, value);

    this->m_renderer->cameraController.moveKeyPressed(cur_key);

}
void LveWindow::keyReleaseEvent(QKeyEvent *e)
{
    if( !this->m_renderer->isRenderModelLoaded() ) return;
    Qt::Key cur_key = (Qt::Key)e->key();
    this->m_renderer->cameraController.moveKeyReleased(cur_key);
}

