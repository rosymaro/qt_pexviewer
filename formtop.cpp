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
    ui->setupUi(this);
    this->m_scene = new QGraphicsScene(this);
    trans.scale(1,-1);

    formtop_cut_all = formtop_menu.addAction(tr("All"));
    formtop_cut_all->setCheckable(true);
    formtop_cut_all->setChecked(true);

    formtop_cut_rigtht = formtop_menu.addAction(tr("Right"));
    formtop_cut_rigtht->setCheckable(true);

    formtop_cut_left = formtop_menu.addAction(tr("Left"));
    formtop_cut_left->setCheckable(true);

    formtop_cut_top = formtop_menu.addAction(tr("Top"));
    formtop_cut_top->setCheckable(true);

    formtop_cut_bottom = formtop_menu.addAction(tr("Bottom"));
    formtop_cut_bottom->setCheckable(true);

}


FormTop::~FormTop()
{
    delete ui;
}

void FormTop::receiveFile(T2D &t2d, int pixel, double um)
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

    this->m_origin_scale = pixel/um;
    this->m_scale = this->m_origin_scale;

    m_scene->setBackgroundBrush(QBrush(QColor(Qt::white)));

    drawing();
}

void FormTop::drawing()
{
    //areaFilterRect();
    this->getRowCol();
    //cout << "r:" << m_begin_row << " " << m_end_row << endl;
    //cout << "c:" << m_begin_col << " " << m_end_col << endl;
    for(int layer = 0 ; layer < (int)rendering_full->LayoutData10by10.size() ; layer++)
    {
        //layer_enviroment Ä ÎπÑÍµêòÏó¨ for skip
        if( rendering_full->LayoutData10by10[layer].checking == 0){continue;}
        /*m_begin_row = 0;
        m_begin_col = 0;
        m_end_col = 3;
        m_end_row = 3;*///for test
        for(int row = m_begin_row ; row < m_end_row ; row++)
        {
            for(int col = m_begin_col ; col < m_end_col ; col++)
            {
                for(int n = 0 ; n < (int)(rendering_full->LayoutData10by10[layer].xy[row][col]).size(); n++)
                {
                    addRectItem(layer,row,col,n,trans);
                }
            }
        }
    }
    m_scene->setSceneRect(0, 0, this->m_scene_standard, this->m_scene_standard);
    ui->graphicsView->setScene(m_scene);

    m_scene->addRect(0,0,100,100, QPen(QBrush(Qt::red), 0), QBrush(Qt::NoBrush));
    m_scene->addRect(50,50,50,50, QPen(QBrush(Qt::blue), 0), QBrush(Qt::NoBrush));
    m_scene->addRect(0,0,50,50, QPen(QBrush(Qt::black), 0), QBrush(Qt::NoBrush));

}

void FormTop::filterDrawing()
{
    areaFilterRect();
    for(int layer = 0 ; layer < (int)rendering_full->LayoutData10by10.size() ; layer++)
    {
        //layer_enviroment Ä ÎπÑÍµêòÏó¨ for skip
        if( rendering_full->LayoutData10by10[layer].checking == 0){continue;}
        for(int row = m_begin_row ; row < m_end_row ; row++)
        {
            for(int col = m_begin_col ; col < m_end_col ; col++)
            {
                for(int n = 0 ; n < (int)(rendering_full->LayoutData10by10[layer].xy_filtered[row][col]).size(); n++)
                {
                    addFilterRectItem(layer,row,col,n,trans);
                }
            }
        }
    }

    m_scene->setSceneRect(0, 0, this->m_scene_standard, this->m_scene_standard);
    ui->graphicsView->setScene(m_scene);

    m_scene->addRect(0,0,100,100, QPen(QBrush(Qt::red), 0), QBrush(Qt::NoBrush));
    m_scene->addRect(50,50,50,50, QPen(QBrush(Qt::blue), 0), QBrush(Qt::NoBrush));
    m_scene->addRect(0,0,50,50, QPen(QBrush(Qt::black), 0), QBrush(Qt::NoBrush));
}

void FormTop::drawingClear()
{
    // Í∏∞Ï°¥àÎäî vector Î•®Í≤®îÏïº : delete Î•ÑÌï¥
    for(auto cur_item : rectItemList)
    {
        delete cur_item;
    }
    rectItemList.clear();
    m_scene->clear();
}

void FormTop::receivePointPos(POS_MONITORING &pos)
{
    this->pos = &pos;
    temp_pos.x = pos.x;
    temp_pos.y = pos.y;
}

void FormTop::changePos()
{    
    if(this->changepos_init_check == false) //temp_pos.x != pos->x || temp_pos.y != pos->y)
    {
        m_min_x_size = pos->x - m_area / m_area_scale;
        m_max_x_size = pos->x + m_area / m_area_scale;
        m_min_y_size = pos->y - m_area / m_area_scale;
        m_max_y_size = pos->y + m_area / m_area_scale;

        temp_pos.x = pos->x;
        temp_pos.y = pos->y;

        this->m_scale = this->m_origin_scale * pos->zoom;
        //cout << "jj " << m_scale/0.1 << endl;

        drawingClear();
        if(rendering_full->m_filter_onoff == false){
            drawing();
        }
        if(rendering_full->m_filter_onoff == true){
            filterDrawing();
        }
    } else{
        this->changepos_init_check = false;
    }
}

void FormTop::addRectItem(int layer, int row, int col, int n, QTransform trans)
{

    QGraphicsRectItem *rectItem = new QGraphicsRectItem;
    rectItemList.push_back(rectItem);

    /*float opacity = rendering_full->LayoutData10by10[layer].color.a;
    opacity = opacity/255;*/

    float x = m_scale*(rendering_full->LayoutData10by10[layer].xy[row][col][n].minx - pos->x) + this->m_scene_standard / 2;
    float y = m_scale*(rendering_full->LayoutData10by10[layer].xy[row][col][n].miny - pos->y) - this->m_scene_standard / 2;
    float w = m_scale*(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx - rendering_full->LayoutData10by10[layer].xy[row][col][n].minx);
    float h = m_scale*(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy - rendering_full->LayoutData10by10[layer].xy[row][col][n].miny);

    rectItem->setRect(x,y,w,h);
    rectItem->setBrush(QBrush(QColor(rendering_full->LayoutData10by10[layer].color.r,
                                     rendering_full->LayoutData10by10[layer].color.g,
                                     rendering_full->LayoutData10by10[layer].color.b,
                                     rendering_full->LayoutData10by10[layer].color.a)));
    //rectItem->setOpacity(opacity);
    //qDebug()<< " rect    : " << rectItem->rect() << ":: row/col : " << row << "/" << col << " " << n;
    //qDebug()<< " opacity : " << rendering_full->LayoutData10by10[layer].color.a << " : " << opacity;

    //cout << x << ":" << y << " " << w << ":" << h << endl;
    rectItem->setZValue(rendering_full->LayoutData10by10[layer].bot);

    rectItem->setTransform(trans);
    m_scene->addItem(rectItem);

    /*QColor formtop_rgba = QColor(rendering_full->LayoutData10by10[layer].color.r,
                                 rendering_full->LayoutData10by10[layer].color.g,
                                 rendering_full->LayoutData10by10[layer].color.b,
                                 rendering_full->LayoutData10by10[layer].color.a);

    m_scene->addRect(x, y, w, h, QPen(QBrush(Qt::NoBrush), 0), QBrush(formtop_rgba));*/
}

void FormTop::addFilterRectItem(int layer, int row, int col, int n, QTransform trans)
{

    QGraphicsRectItem *rectItem = new QGraphicsRectItem;
    rectItemList.push_back(rectItem);

    /*float opacity = rendering_full->LayoutData10by10[layer].color.a;
    opacity = opacity/255;*/

    float x = m_scale*(rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].minx - pos->x) + this->m_scene_standard / 2;
    float y = m_scale*(rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].miny - pos->y) - this->m_scene_standard / 2;
    float w = m_scale*(rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].maxx - rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].minx);
    float h = m_scale*(rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].maxy - rendering_full->LayoutData10by10[layer].xy_filtered[row][col][n].miny);

    rectItem->setRect(x,y,w,h);
    rectItem->setBrush(QBrush(QColor(rendering_full->LayoutData10by10[layer].color.r,
                                     rendering_full->LayoutData10by10[layer].color.g,
                                     rendering_full->LayoutData10by10[layer].color.b,
                                     rendering_full->LayoutData10by10[layer].color.a)));

    //rectItem->setOpacity(opacity);
    //qDebug()<< " rect    : " << rectItem->rect() << ":: row/col : " << row << "/" << col << " " << n;
    //qDebug()<< " opacity : " << rendering_full->LayoutData10by10[layer].color.a << " : " << opacity;

    rectItem->setZValue(rendering_full->LayoutData10by10[layer].bot);

    rectItem->setTransform(trans);
    m_scene->addItem(rectItem);
}

int FormTop::extPos(double *pos, double *min, double *max)
{
    double area = m_area / m_area_scale; // 0.05 = 50/1000
    int pos_col_row = (*pos-*min)/m_block_size; // 30.0413/10 = 3
    int max_col_row = (*max-*min)/m_block_size;


    //block ºÎ°ú êÎ•∏ Í≤ΩÍ≥ÑÎ©Í∑ºÏ≤òàÏùÑ Í≤ΩÏö∞ external Ï∂îÍ
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
    double minx = 0;
    double maxx = 0;
    double miny = 0;
    double maxy = 0;

    if(m_filter_right == true){
        if(rendering_full->LayoutData10by10[layer].xy[row][col][n].minx >= pos->x){return;}
        if(rendering_full->LayoutData10by10[layer].xy[row][col][n].minx < pos->x && rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx >= pos->x) //area Î≥¥Îã§ òÏñ¥Í∞ÄÎ©êÎ¶Ñ
        {
            maxx = pos->x;
        }
        else
        {
            maxx = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx;
        }
        minx = rendering_full->LayoutData10by10[layer].xy[row][col][n].minx;
        miny = rendering_full->LayoutData10by10[layer].xy[row][col][n].miny;
        maxy = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy;
    }
    if(m_filter_left == true){
        if(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx <= pos->x){return;}
        if(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx > pos->x && rendering_full->LayoutData10by10[layer].xy[row][col][n].minx <= pos->x) //area Î≥¥Îã§ òÏñ¥Í∞ÄÎ©êÎ¶Ñ
        {
            minx = pos->x;
        }
        else
        {
            minx = rendering_full->LayoutData10by10[layer].xy[row][col][n].minx;
        }
        maxx = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx;
        miny = rendering_full->LayoutData10by10[layer].xy[row][col][n].miny;
        maxy = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy;
    }
    if(m_filter_top == true){
        if(rendering_full->LayoutData10by10[layer].xy[row][col][n].miny >= pos->y){return;}
        if(rendering_full->LayoutData10by10[layer].xy[row][col][n].miny < pos->y && rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy >= pos->y) //area Î≥¥Îã§ òÏñ¥Í∞ÄÎ©êÎ¶Ñ
        {
            maxy = pos->y;
        }
        else
        {
            maxy = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy;
        }
        minx = rendering_full->LayoutData10by10[layer].xy[row][col][n].minx;
        maxx = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx;
        miny = rendering_full->LayoutData10by10[layer].xy[row][col][n].miny;
    }
    if(m_filter_bottom == true){
        if(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy <= pos->y){return;}
        if(rendering_full->LayoutData10by10[layer].xy[row][col][n].maxy > pos->y && rendering_full->LayoutData10by10[layer].xy[row][col][n].miny <= pos->y) //area Î≥¥Îã§ òÏñ¥Í∞ÄÎ©êÎ¶Ñ
        {
            miny = pos->y;
        }
        else
        {
            miny = rendering_full->LayoutData10by10[layer].xy[row][col][n].miny;
        }
        minx = rendering_full->LayoutData10by10[layer].xy[row][col][n].minx;
        maxx = rendering_full->LayoutData10by10[layer].xy[row][col][n].maxx;
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
    this->getRowCol();

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

    /*int pos_col = (pos->x - rendering_full->LayoutMinMax.minx)/m_block_size; //ùÏù¥ ÑÏöîúÎ   23.3841 / 10 = 2.33841 -> int 2
    int ext_col = extPos(&pos->x, &rendering_full->LayoutMinMax.minx, &rendering_full->LayoutMinMax.maxx);
    m_begin_col = qMin(pos_col,ext_col);
    m_end_col = qMax(pos_col,ext_col)+1;

    int pos_row = (pos->y - rendering_full->LayoutMinMax.miny)/m_block_size;
    int ext_row = extPos(&pos->y, &rendering_full->LayoutMinMax.miny,&rendering_full->LayoutMinMax.maxy);
    m_begin_row = qMin(pos_row,ext_row);
    m_end_row = qMax(pos_col,ext_row)+1;*/

    //layer
    for(int layer = 0 ; layer < (int)rendering_full->LayoutData10by10.size() ; layer++)
    {
        //layer_enviroment Ä ÎπÑÍµêòÏó¨ for skip

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

void FormTop::getRowCol(){
    int pos_col = (pos->x - rendering_full->LayoutMinMax.minx)/m_block_size; //ùÏù¥ ÑÏöîúÎ   23.3841 / 10 = 2.33841 -> int 2
    if(pos_col - 1 >= 0){
        m_begin_col = pos_col - 1;
    }else{
        m_begin_col = 0;
    }

    if(pos_col + 2 <= rendering_full->col){
        m_end_col = pos_col + 2;
    }else{
        m_end_col = rendering_full->col;
    }

    int pos_row = (pos->y - rendering_full->LayoutMinMax.miny)/m_block_size;
    if(pos_row - 1 >= 0){
        m_begin_row = pos_row - 1;
    }else{
        m_begin_row = 0;
    }

    if(pos_row + 2 <= rendering_full->row){
        m_end_row = pos_row + 2;
    }else{
        m_end_row = rendering_full->row;
    }
}

void FormTop::wheelEvent(QWheelEvent *event){
    int delta = event->delta();

    // §ÌÅ¨Î°Í∞íÏóê ∞Îùº §ÏÍ∞íÏùÑ Î≥ÄÍ≤
    if (delta > 0) {
        this->pos->zoom *= 1.01; // 10% ïÎ
    } else if (delta < 0) {
        this->pos->zoom /= 1.01; // 10% Ï∂ïÏÜå
    }
    //pos->zoom = m_scale;
    //cout << m_scale << endl;
    cout << this->pos->zoom << endl;
    //emit signalChangePos("test", *this->pos);
}

void FormTop::contextMenuEvent(QContextMenuEvent *event){
    QAction* selectedItem = formtop_menu.exec(event->globalPos());
    if (selectedItem == formtop_cut_all) {
        rendering_full->m_filter_onoff = false;
        m_filter_right = false;
        m_filter_left = false;
        m_filter_top = false;
        m_filter_bottom = false;
        this->changePos();
    }
    if (selectedItem == formtop_cut_rigtht) {
        rendering_full->m_filter_onoff = true;
        m_filter_right = true;
        m_filter_left = false;
        m_filter_top = false;
        m_filter_bottom = false;
        this->changePos();
    }
    if (selectedItem == formtop_cut_left) {
        rendering_full->m_filter_onoff = true;
        m_filter_right = false;
        m_filter_left = true;
        m_filter_top = false;
        m_filter_bottom = false;
        this->changePos();
    }
    if (selectedItem == formtop_cut_top) {
        rendering_full->m_filter_onoff = true;
        m_filter_right = false;
        m_filter_left = false;
        m_filter_top = true;
        m_filter_bottom = false;
        this->changePos();
    }
    if (selectedItem == formtop_cut_bottom) {
        rendering_full->m_filter_onoff = true;
        m_filter_right = false;
        m_filter_left = false;
        m_filter_top = false;
        m_filter_bottom = true;
        this->changePos();
    }
    filterMenuCheck();
}

void FormTop::filterMenuCheck(){
    formtop_cut_all->setChecked(!rendering_full->m_filter_onoff);
    formtop_cut_rigtht->setChecked(m_filter_right);
    formtop_cut_left->setChecked(m_filter_left);
    formtop_cut_top->setChecked(m_filter_top);
    formtop_cut_bottom->setChecked(m_filter_bottom);
}









