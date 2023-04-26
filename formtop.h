#ifndef FORMTOP_H
#define FORMTOP_H

#include "T2D.h"
#include "defineParam.h"
#include <QDialog>
#include <QGraphicsItem>
#include <QVector>
#include <QGraphicsRectItem>
#include <QMenu>
#include <QContextMenuEvent>

namespace Ui {
class FormTop;
}

class FormTop : public QDialog
{
    Q_OBJECT

public:
    explicit FormTop(QWidget *parent = nullptr);
    ~FormTop();

    void receiveFile(T2D &t2d, int pixel, double um);
    void drawing();
    void filterDrawing();
    void drawingClear();
    void receivePointPos(POS_MONITORING &pos);
    void changePos();
//    void receiveFile(QVector<QList<float>> **mapFile);

private:
    void addRectItem(int layer, int row, int col, int n, QTransform trans);
    void addFilterRectItem(int layer, int row, int col, int n, QTransform trans);
    int extPos(double *pos, double *min, double *max);
    void filterRenderingData(int layer, int row, int col, int n);
    void areaFilterRect();
    void getRowCol();
    Ui::FormTop *ui;
    T2D *rendering_full;        
    POS_MONITORING *pos;
    POS_MONITORING temp_pos;
    QGraphicsScene *m_scene;
    QVector<QGraphicsRectItem*> rectItemList;
    QTransform trans;
    int m_begin_x = 0, m_begin_y = 0, m_end_x = 1, m_end_y = 1, m_scale = 500;
    float m_min_x = 0, m_min_y =0, m_max_x =0, m_max_y=0;
    bool init_check = true;

    int m_begin_row = 0, m_end_row = 1, m_begin_col = 0, m_end_col = 1;
    int m_block_size = 10; //10um
    double m_area = 50, m_area_scale = 100 ; // 50/1000 = 0.05um(0.001~0.099)
    double m_min_x_size = -0.5, m_max_x_size = 0.5, m_min_y_size = -0.5, m_max_y_size = 0.5;

    double m_origin_scale;
    bool changepos_init_check = true;
    int m_scene_standard = 100;

    QMenu formtop_menu;
    QAction* formtop_cut_all;
    QAction* formtop_cut_rigtht;
    QAction* formtop_cut_left;
    QAction* formtop_cut_top;
    QAction* formtop_cut_bottom;

    //bool m_filter_onoff = false;
    bool m_filter_right = false;
    bool m_filter_left = false;
    bool m_filter_top = false;
    bool m_filter_bottom = false;

    void filterMenuCheck();

signals:
    void signalChangePos(QString func, POS_MONITORING pos);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // FORMTOP_H
