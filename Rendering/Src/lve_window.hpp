#ifndef LVEWINDOW_H
#define LVEWINDOW_H
#include <vulkan/vulkan.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <QVulkanWindow>
#include "defineParam.h"
//#include <QVulkanWindowRenderer>

//#include "dtaorendersystem.h"

class QVulkanWidowRenderer;
class DtaoRenderSystem;

class LveWindow : public QVulkanWindow
{
    Q_OBJECT

public:
    LveWindow();
    ~LveWindow();

    LveWindow(const LveWindow &) = delete;
    LveWindow &operator=(const LveWindow &) = delete;

public:
    QVulkanWindowRenderer *createRenderer() override;
    //QVulkanWindowRenderer * getRenderer(){ return this->m_renderer;}
    //LveCamera* camera;
    void emitStatus(float value);

public:
    DtaoRenderSystem * getRenderer(){return this->m_renderer;}

private:
    DtaoRenderSystem *m_renderer;
    //QVulkanWindowRenderer * m_renderer;

    void wheelEvent(QWheelEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;

    bool keyCtrl = false;
    bool keyShift = false;
    bool keyAlt = false;

    int m_mouseButton = 0;


    QPoint m_lastPos;

signals:
    void signalInfoText(QString funcName, POS_MONITORING value);
};

#endif // LVEWINDOW_H
