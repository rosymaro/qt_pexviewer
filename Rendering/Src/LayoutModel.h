#pragma once

#include "lve_model.hpp"
#include "T2D.h"

class LayoutModel : public LveModel
{
public:
    LayoutModel(LveDevice& device, MODEL_TYPE model_type, T2D &t2d);
    ~LayoutModel();

    LayoutModel() = delete;
    LayoutModel(const LayoutModel&) = delete;
    LayoutModel& operator=(const LayoutModel&) = delete;

private:
    LayoutDataManager layout_data;

public:
    virtual void makeRenderingData(T2D &t2d);
    virtual void loadData(T2D &t2d);
    virtual void makeVertices();
    virtual void makeIndices();

    void makeCubeVertices();
    void makeIndicesForFace();
    void makeIndicesForEdge();

    void makeCubeVertices_map();
    void makeIndicesForFace_map();
    void makeIndicesForEdge_map();


    LayoutDataManager* getLayoutDataManager() {return &this->layout_data;}

};



