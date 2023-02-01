#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./trianglerenderer.h"

#include <QMainWindow>
#include <QVulkanWindow>
#include <QDockWidget>
#include "formhier.h"
#include "formlayer.h"
#include "formtop.h"
#include "forminfo.h"
#include "formmap.h"

class VulkanWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; class LayerForm; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(VulkanWindow *w);
    ~MainWindow();

    void moveEvent(QMoveEvent *e) override;

public slots:

    void slotInfoText(QString funcName, float value);

private slots:
    void on_pushButton_clicked();

signals:
    void signalInfoText(QString funcName, float value);


private:
    QVulkanWindow *m_window;
    Ui::MainWindow *ui;
    QString statusText;
    FormHier *formHier;
    FormLayer *formLayer;
    FormTop *formTop;
    FormInfo *formInfo;
    FormMap *formMap;
    QDockWidget *dockHier;
    QDockWidget *dockLayer;
    QDockWidget *dockTop;
    QDockWidget *dockInfo;
    QDockWidget *dockMap;
    int start = 0;
    int leftMain;
    int topMain;
    int heightMain;
    int widthMain;

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
    void signalInfoText(QString funcName, float value);
};



#endif // MAINWINDOW_H
