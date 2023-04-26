#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Rendering/Src/lve_window.hpp"

#include <QMainWindow>
#include <QVulkanWindow>
#include <QDockWidget>
#include "formhier.h"
#include "formlayer.h"
#include "formtop.h"
#include "forminfo.h"
#include "formmap.h"
#include "pexdialog.h"
#include "filedb.h"
#include "all_data.h"
#include <QVector>
#include "defineParam.h"

#include "dev_dialog.h"

#include "T2D.h"

class LveWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; class LayerForm; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(LveWindow *w);
    ~MainWindow();

    void shareGeo(QRect size);
    void devDialog();

    T2D t2d;

public slots:

    void slotInfoText(QString funcName, POS_MONITORING value);
    void inputLayerStatus(QString text);
    void on_actionOpen_file_triggered();
    void inputPosInformation();

signals:
    void signalInfoText(QString funcName, float value);
    void sendSelectFileName(QString file_name);



private slots:
    void on_actionOpen_Layout_triggered();
    void on_actionOpen_DB_triggered();
    void on_actionOpen_PEX_triggered();
    void slotDirectlyInputPos();
    void slotStatusBarMsg(QString msg);

    void on_actionTop_triggered();
    void on_actionDemo_triggered();
    void on_docker_TopView_triggered();
    void on_docker_TopView_stateChanged(bool);
    void on_docker_Map_triggered();
    void on_docker_Map_stateChanged(bool);
    void on_docker_Info_triggered();
    void on_docker_Info_stateChanged(bool);
    void on_docker_Hier_triggered();
    void on_docker_Hier_stateChanged(bool);
    void on_docker_Layer_triggered();
    void on_docker_Layer_stateChanged(bool);
    void on_toggle_top_triggered();

    void on_PEX_window_triggered();

    void on_actionDialog_on_off_triggered();

private:
    void sendPositionDataToDocker();
    LveWindow *m_window;
    Ui::MainWindow *ui;
    QString statusText;
    FormHier *formHier;
    FormLayer *formLayer;
    FormTop *formTop;
    FormInfo *formInfo;
    FormMap *formMap;
    PexDialog *pexdialog;
    QDockWidget *dockHier;
    QDockWidget *dockLayer;
    QDockWidget *dockMap;
    QDockWidget *dockTop;
    QDockWidget *dockInfo;
    SuperItem *superItem;
    QRect *windowSize;
    FileDb *fileDb;
    all_data *input_dataS;
    QStringList split_data;
    POS_MONITORING pos;
    POS_MONITORING pos_past;
    QByteArray dock_widget_geometry;
    QByteArray dock_widget_state;

//dev_dialog
    DevDialog *dev_dialog;
    bool is_pos_checked = true;

};

#endif // MAINWINDOW_H
