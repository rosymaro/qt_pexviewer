#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./trianglerenderer.h"

#include <QMainWindow>
#include <QVulkanWindow>

class VulkanWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(VulkanWindow *w);
    ~MainWindow();

public slots:

    void inputStatus(QString text);

private:
    QVulkanWindow *m_window;
    Ui::MainWindow *ui;
    QString statusText;

};

// Graphics output
class VulkanRenderer : public TriangleRenderer
{
public:
    VulkanRenderer(VulkanWindow *w);
};

// Graphics output -> Qt
class VulkanWindow : public QVulkanWindow
{
    Q_OBJECT

public:
    QVulkanWindowRenderer *createRenderer() override;

private:
    VulkanRenderer *m_renderer;
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
    void outputStatus(QString funcValue);
};

#endif // MAINWINDOW_H
