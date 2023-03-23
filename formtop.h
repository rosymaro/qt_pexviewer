#ifndef FORMTOP_H
#define FORMTOP_H

#include "T2D.h"
#include "defineParam.h"
#include <QDialog>
#include <QGraphicsItem>
#include <QVector>
#include <QGraphicsRectItem>

namespace Ui {
class FormTop;
}

class FormTop : public QDialog
{
    Q_OBJECT

public:
    explicit FormTop(QWidget *parent = nullptr);
    ~FormTop();

    void receiveFile(T2D &t2d);
    void drawing();
    void drawingClear();
    void slotPos(POS_MONITORING &pos);
    void changePos();
//    void receiveFile(QVector<QList<float>> **mapFile);

private slots:
    void on_horizontalSlider_valueChanged(int value);

private:
    void addRectItem(int layer, int row, int col, int n, QTransform trans);    
    int extPos(double *pos, double *min, double *max);
    void filterRenderingData(int layer, int row, int col, int n);
    void areaFilterRect();
    Ui::FormTop *ui;
    T2D *rendering_full;        
    POS_MONITORING *pos;
    POS_MONITORING temp_pos;
    QGraphicsScene *m_scene;
    QVector<QGraphicsRectItem*> rectItemList;
    QTransform trans;
    int m_begin_x = 0, m_begin_y = 0, m_end_x = 1, m_end_y = 1, m_scale = 500;
    float m_min_x = 0, m_min_y =0, m_max_x =0, m_max_y=0;
    int error_temp = 0;

    int m_begin_row = 0, m_end_row = 1, m_begin_col = 0, m_end_col = 1;
    int m_block_size = 10; //10um
    double m_area = 50, m_area_scale = 100 ; // 50/1000 = 0.05um(0.001~0.099)
    double m_min_x_size = -0.5, m_max_x_size = 0.5, m_min_y_size = -0.5, m_max_y_size = 0.5;

};

#endif // FORMTOP_H
