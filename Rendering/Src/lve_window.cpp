
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

void LveWindow::mousePressEvent(QMouseEvent *e)
{
    m_mouseButton = e->buttons();
    m_lastPos = e->pos();
}

void LveWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_mouseButton = 0;
    QString funcName = "mouseRelease";
    float value = 0;
    emit signalInfoText(funcName, value);
}

void LveWindow::mouseMoveEvent(QMouseEvent *e)
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

void LveWindow::keyPressEvent(QKeyEvent *e)
{
    Qt::Key cur_key = (Qt::Key)e->key();

    //const float amount = e->modifiers().testFlag(Qt::ShiftModifier) ? 1.0f : 0.1f;
    //QString funcName;
    //float value = 0;
    /*
    //switch (e->key()) {
    switch (cur_key) {
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

    qDebug()<< "Pressed Key : " << cur_key;
    */
    /*
    this->m_renderer->cameraController.moveCamera(
                cur_key,
                1.0,
                this->m_renderer->camera,
                this->m_renderer->getRenderScale()
                );
                */
    this->m_renderer->cameraController.moveKeyPressed(cur_key);

}
void LveWindow::keyReleaseEvent(QKeyEvent *e)
{
    Qt::Key cur_key = (Qt::Key)e->key();
    this->m_renderer->cameraController.moveKeyReleased(cur_key);
    /*
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
    */
}

