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
#include "filedb.h"
#include "all_data.h"
#include <QVector>

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

    T2D t2d;

public slots:

    void slotInfoText(QString funcName, float value);
    void inputLayerStatus(QString text);
    void on_actionOpen_file_triggered();

signals:
    void signalInfoText(QString funcName, float value);
    void sendSelectFileName(QString file_name);


private slots:
    void on_actionOpen_Map_File_triggered();

    void on_actionOpen_Layout_triggered();

    void on_actionOpen_DB_triggered();

private:
    LveWindow *m_window;
    Ui::MainWindow *ui;
    QString statusText;
    FormHier *formHier;
    FormLayer *formLayer;
    FormTop *formTop;
    FormInfo *formInfo;
    FormMap *formMap;
    SuperItem *superItem;
    QRect *windowSize;
    FileDb *fileDb;
    all_data *input_dataS;
    QStringList split_data;
};

#endif // MAINWINDOW_H
