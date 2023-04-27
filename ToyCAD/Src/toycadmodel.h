#ifndef TOYCADMODEL_H
#define TOYCADMODEL_H

#include "Rendering/Src/lve_model.hpp"

#include <iostream>
#include <vector>


enum TOYCADINFO_INDEX {
    TOYCADINFO_IDX_TYPE = 0,
    TOYCADINFO_IDX_LAYER,
    TOYCADINFO_IDX_XMIN,
    TOYCADINFO_IDX_YMIN,
    TOYCADINFO_IDX_XMAX,
    TOYCADINFO_IDX_YMAX,
    TOYCADINFO_IDX_ZMIN,
    TOYCADINFO_IDX_ZMAX,
    TOYCADINFO_IDX_XSLOPE,
    TOYCADINFO_IDX_YSLOPE,
    TOYCADINFO_IDX_DEFAULT,
};

enum TOYCAD_TYPE_LIST {
    TOYCAD_TYPE_RECT = 0,
    TOYCAD_TYPE_CIRCLE = 1,
    TOYCAD_TYPE_DEFAULT = 99
};

struct toycad_data {
    string type;
    string layer;
    double minx;
    double miny;
    double maxx;
    double maxy;
    double minz;
    double maxz;
    double slopex;
    double slopey;
};

struct toycad_coord3d {
    double x;
    double y;
    double z;
};

class ToyCADModel : public LveModel
{
public:
    ToyCADModel(LveDevice& device, MODEL_TYPE model_type);
    ~ToyCADModel();

    ToyCADModel() = delete;
    ToyCADModel(const ToyCADModel&) = delete;
    ToyCADModel& operator=(const ToyCADModel&) = delete;

public:
    virtual void makeRenderingData(const QString& file_path = "");
    virtual void loadData(const string& file_path = "");
    virtual void makeVertices();
    virtual void makeIndices();

private:
    void makeCubeVertices();
    void makeCylinderVertices();
    void makeIndicesForFace();
    //void makeIndicesForEdge();

public:
    void setNumPointsForCircle(unsigned int num_){this->num_points_for_circle=num_;}
private:
    unsigned int num_points_for_circle;
    std::vector<toycad_data> cad_datas;



public:
    void printAllData();
};

#endif // TOYCADMODEL_H
