#include "formtop.h"
#include "ui_formtop.h"
#include <QGraphicsItem>
#include <QScrollBar>
#include <QVector>
#include <QMap>
#include <iostream>
#include <QDebug>


FormTop::FormTop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormTop)
{
    int slider_init_value = 50;

    ui->setupUi(this);
    this->m_scene = new QGraphicsScene(this);
    trans.scale(1,-1);
    ui->horizontalSlider->setValue(slider_init_value);

}


FormTop::~FormTop()
{
    delete ui;
}

void FormTop::receiveFile(T2D &t2d)
{
    drawingClear();
    m_scene->clear();
    rendering_full = &t2d;            
    for(int layer = 0 ; layer < (int)rendering_full->LayoutData10by10.size() ; layer++)
    {
        for(int row = 0 ; row < (int)rendering_full->LayoutData10by10[layer].xy.size() ; row++)
        {
            rendering_full->LayoutData10by10[layer].xy_filtered.resize(rendering_full->LayoutData10by10[layer].xy.size());
            for(int col = 0 ; col < (int)rendering_full->LayoutData10by10[layer].xy[row].size() ; col++)
            {
                rendering_full->LayoutData10by10[layer].xy_filtered[row].resize(rendering_full->LayoutData10by10[layer].xy[row].size());
            }
        }
    }



    m_scene->setBackgroundBrush(QBrush(QColor(Qt::white)));

    drawing();
}

void FormTop::drawing()
{
    areaFilterRect();
    if(m_begin_row == 0 && m_begin_col ==0 && zero_point)
    {
        addZeroPoint();
        zero_point = false;
    }

    for(int layer = 0 ; layer < (int)rendering_full->LayoutData10by10.size() ; layer++)
    {
        //layer_enviroment  ë¹êµì¬ for skip
        if( rendering_full->LayoutData10by10[layer].checking == 0){continue;}
        for(int row = m_begin_row ; row < m_end_row ; row++)
        {
            for(int col = m_begin_col ; col < m_end_col ; col++)
            {
                for(int n = 0 ; n < (int)(rendering_full->LayoutData10by10[layer].xy_filtered[row][col]).size(); n++)
                {
                    addRectItem(layer,row,col,n,trans);
                }
            }
        }
    }

    ui->graphicsView->setScene(m_scene);
}

void FormTop::drawingClear()
{
    // ê¸°ì¡´ë vector ë¥¨ê²¨ì¼ : delete ë¥í´
    for(auto cur_item : rectItemList)
    {
        delete cur_item;
    }
    rectItemList.clear();
    zero_point = true;

}

void FormTop::receivePointPos(POS_MONITORING &pos)
{
    this->pos = &pos;
    temp_pos.x = pos.x;
    temp_pos.y = pos.y;
}

void FormTop::changePos()
{    
    if(temp_pos.x != pos->x || temp_pos.y != pos->y)
    {
        m_min_x_size = pos->x - m_area / m_area_scale;
        m_max_x_size = pos->x + m_area / m_area_scale;
        m_min_y_size = pos->y - m_area / m_area_scale;
        m_max_y_size = pos->y + m_area / m_area_scale;

        temp_pos.x = pos->x;
        temp_pos.y = pos->y;

        drawingClear();
        drawing();
        ui->graphicsView->setTransform(QTransform().translate(m_scale*pos->x, -1*m_scale*pos->y));
        qDebug() << "transpose : "<<QTransform().transposed();
    }
}

void FormTop::on_horizontalSlider_valueChanged(int value)
{
    m_area = value;

    if(init_check)
    {
        qDebug() << "pos init";
        m_min_x_size = 0 - m_area / m_area_scale;
        m_max_x_size = 0 + m_area / m_area_scale;
        m_min_y_size = 0 - m_area / m_area_scale;
        m_max_y_size = 0 + m_area / m_area_scale;
        init_check = false;
    }
    else
    {
        qDebug() << "pos working";
        m_min_x_size = pos->x - m_area / m_area_scale;
        m_max_x_size = pos->x + m_area / m_area_scale;
        m_min_y_size = pos->y - m_area / m_area_scale;
        m_max_y_size = pos->y + m_area / m_area_scale;

        drawingClear();
        drawing();
    }
}

void FormTop::addZeroPoint()
{
    qDebug() << "drawing or not?";
    QGraphicsRectItem *zeroItem = new QGraphicsRectItem;
    rectItemList.push_back(zeroItem);
    zeroItem->setRect(m_scale*rendering_full->LayoutMinMax.minx,m_scale*rendering_full->LayoutMinMax.miny,m_scale*0.01,m_scale*0.01);
    zeroItem->setBrush(QBrush(QColor(Qt::red)));
    zeroItem->setZValue(9999);
    m_scene->addItem(zeroItem);
}


void FormTop::addRectItem(int layer, int row, int col, int n, QTransform trans)
{
    QGraphicsRectItem *rectItem = new QGraphicsRectItem;
    rectItemList.push_back(rectItem);

    float opacity = rendering_full->LayoutData10by10[layer].color.a;
    opacity = opacity/255;

    float x = m_scale*(rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].minx);
    float y = m_scale*(rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].miny);
    float w = m_scale*(rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].maxx-rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].minx);
    float h = m_scale*(rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].maxy-rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].miny);

    rectItem->setRect(x,y,w,h);
    rectItem->setBrush(QBrush(QColor(rendering_full->LayoutData10by10[layer].color.r,rendering_full->LayoutData10by10[layer].color.g,rendering_full->LayoutData10by10[layer].color.b,rendering_full->LayoutData10by10[layer].color.a)));
    rectItem->setOpacity(opacity); 
    rectItem->setZValue(rendering_full->LayoutData10by10[layer].bot);

    rectItem->setTransform(trans);
    m_scene->addItem(rectItem);
}

int FormTop::extPos(double *pos, double *min, double *max)
{
    double area = m_area / m_area_scale; // 0.05 = 50/1000
    int pos_col_row = (*pos-*min)/m_block_size; // 30.0413/10 = 3
    int max_col_row = (*max-*min)/m_block_size;


    //block ¼ë¡ ë¥¸ ê²½ê³ë©ê·¼ì²ì ê²½ì° external ì¶ê
    if ((*pos - *min)<area && pos_col_row > 0) // 30.0413 - 3*10 = 0.0413 < 0.05
    {
        return pos_col_row - 1;
    }
    else if ((*max - *pos)<area && pos_col_row < max_col_row) // 39.9811 col 3 : (3+1)*10 - 39.9811 = 0.0189 < 0.05
    {
        return pos_col_row + 1;
    }

    return pos_col_row;
}

void FormTop::filterRenderingData(int layer, int row, int col, int n)
{
    if(rendering_full->LayoutData10by10[layer].xy[row][col][n].minx > m_max_x_size){return;}
    if(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx < m_min_x_size){return;}
    if(rendering_full->LayoutData10by10[layer].xy[row][col][n].miny > m_max_y_size){return;}
    if(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy < m_min_y_size){return;}

    double minx, maxx, miny, maxy;

    if(rendering_full->LayoutData10by10[layer].xy[row][col][n].minx < m_min_x_size) //area ë³´ë¤ ì´ê°ë©ë¦
    {
        minx = m_min_x_size;
    }
    else
    {
        minx = rendering_full->LayoutData10by10[layer].xy[row][col][n].minx;
    }

    if(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx > m_max_x_size) //area ë³´ë¤ ì´ê°ë©ë¦
    {
        maxx = m_max_x_size;
    }
    else
    {
        maxx = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx;
    }

    if(rendering_full->LayoutData10by10[layer].xy[row][col][n].miny < m_min_y_size) //area ë³´ë¤ ì´ê°ë©ë¦
    {
        miny = m_min_y_size;
    }
    else
    {
        miny = rendering_full->LayoutData10by10[layer].xy[row][col][n].miny;
    }

    if(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy > m_max_y_size) //area ë³´ë¤ ì´ê°ë©ë¦
    {
        maxy = m_max_y_size;
    }
    else
    {
        maxy = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy;
    }

    B_BOX temp;
    temp.minx = minx;
    temp.miny = miny;
    temp.maxx = maxx;
    temp.maxy = maxy;

    rendering_full->LayoutData10by10[layer].xy_filtered[row][col].push_back(temp);
}

void FormTop::areaFilterRect()
{
    //filter BBOX clear

    for(int layer = 0 ; layer < (int)rendering_full->LayoutData10by10.size() ; layer++)
    {
        for(int row = m_begin_row ; row < m_end_row ; row++)
        {
            for(int col = m_begin_col ; col < m_end_col ; col++)
            {
                rendering_full->LayoutData10by10[layer].xy_filtered[row][col].clear();
            }
        }
    }

    int pos_col = (pos->x - rendering_full->LayoutMinMax.minx)/m_block_size; //ì´ ìë   23.3841 / 10 = 2.33841 -> int 2
    int ext_col = extPos(&pos->x, &rendering_full->LayoutMinMax.minx, &rendering_full->LayoutMinMax.maxx);
    m_begin_col = qMin(pos_col,ext_col);
    m_end_col = qMax(pos_col,ext_col)+1;

    int pos_row = (pos->y - rendering_full->LayoutMinMax.miny)/m_block_size;
    int ext_row = extPos(&pos->y, &rendering_full->LayoutMinMax.miny,&rendering_full->LayoutMinMax.maxy);
    m_begin_row = qMin(pos_row,ext_row);
    m_end_row = qMax(pos_col,ext_row)+1;


    //layer
    for(int layer = 0 ; layer < (int)rendering_full->LayoutData10by10.size() ; layer++)
    {
        //layer_enviroment  ë¹êµì¬ for skip

        if( rendering_full->LayoutData10by10[layer].checking == 0){continue;}

        for(int row = m_begin_row ; row < m_end_row ; row++)
        {
            for(int col = m_begin_col ; col < m_end_col ; col++)
            {
                for(int n = 0 ; n < (int)(rendering_full->LayoutData10by10[layer].xy[row][col]).size(); n++)
                {                
                    filterRenderingData(layer,row,col,n);
                }
            }
        }
    }

}










