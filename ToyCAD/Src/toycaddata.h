#ifndef TOYCADDATA_H
#define TOYCADDATA_H

#include <iostream>
#include <vector>


enum TOYCAD_DATA_INDEX {
    TOYCAD_DATA_IDX_TYPE = 0,
    TOYCAD_DATA_IDX_LAYER,
    TOYCAD_DATA_IDX_MINX,
    TOYCAD_DATA_IDX_MINY,
    TOYCAD_DATA_IDX_MAXX,
    TOYCAD_DATA_IDX_MAXY,
    TOYCAD_DATA_IDX_MINZ,
    TOYCAD_DATA_IDX_MAXZ,
    TOYCAD_DATA_IDX_COLOR_R,
    TOYCAD_DATA_IDX_COLOR_G,
    TOYCAD_DATA_IDX_COLOR_B,
    TOYCAD_DATA_IDX_COLOR_A,
    TOYCAD_DATA_IDX_SLOPEX,
    TOYCAD_DATA_IDX_SLOPEY,
    TOYCAD_DATA_IDX_ROTATE,
    TOYCAD_DATA_IDX_DEFAULT
};

enum TOYCAD_BBOX_INDEX {
    TOYCAD_BBOX_IDX_MINX = 0,
    TOYCAD_BBOX_IDX_MAXX,
    TOYCAD_BBOX_IDX_MINY,
    TOYCAD_BBOX_IDX_MAXY,
    TOYCAD_BBOX_IDX_MINZ,
    TOYCAD_BBOX_IDX_MAXZ,
    TOYCAD_BBOX_IDX_DEFAULT
};

enum TOYCAD_TYPE_LIST {
    TOYCAD_TYPE_RECT = 0,
    TOYCAD_TYPE_CIRCLE,
    TOYCAD_TYPE_FAN,
    TOYCAD_TYPE_HEXAGON,
    TOYCAD_TYPE_OCTAGON,
    TOYCAD_TYPE_DEFAULT = 99
};

struct TOYCAD_DATA {
    std::string type;
    std::string layer;
    double minx;
    double miny;
    double maxx;
    double maxy;
    double minz;
    double maxz;
    double color_r;
    double color_g;
    double color_b;
    double color_a;
    double slopex;
    double slopey;
    double rotation_degree;
};

struct TOYCAD_BBOX{
    double min_x;
    double max_x;
    double min_y;
    double max_y;
    double min_z;
    double max_z;
};


class ToyCADData
{
public:
    ToyCADData();
    ~ToyCADData();

public:
    std::vector<TOYCAD_DATA>* getCADData(){ return &this->cad_data;}
    void loadToyCADData(const std::string& file_path);
    TOYCAD_BBOX& getDataBBox(){ return this->data_bbox;}
    double getScale(){return this->scale;}

private:
    std::vector<TOYCAD_DATA> cad_data;
    TOYCAD_BBOX data_bbox;
    double scale;

private:
    void calculateScale();
    void printAllData();
};

#endif // TOYCADDATA_H
