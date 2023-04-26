#pragma once

#include "lve_model.hpp"


class PEXResistorModel : public LveModel
{
public:

    PEXResistorModel(LveDevice& device, MODEL_TYPE model_type, const QString& data_file = "");
    ~PEXResistorModel();

    PEXResistorModel() = delete;
    PEXResistorModel(const PEXResistorModel&) = delete;
    PEXResistorModel& operator=(const PEXResistorModel&) = delete;

private:
    PEXResistorDataManager pex_data;
    double res_cube_thickness;


public:
    virtual void makeRenderingData(const QString& file_path = "");
    virtual void loadData(const QString& file_path = "");
    virtual void groupResistorOfNode();
    virtual void increaseNumOfResistorGroup();
    virtual void decreaseNumOfResistorGroup();
    virtual void setDirectionOfResistor(PEXResDirection _direction);
    virtual void makeVertices();
    virtual void makeIndices();
    virtual void makeIndicesForEdge_map();

    double percentileOfResistance(const PEXResistor& in_res);

    void makeCubeVertices();
    void makeCubes();
    void makeCube(const PEXResistor& in_res, cube_info& out_cube, PEXResDirection direction);
    void setResCubeThickness(double value) { this->res_cube_thickness = value; }

private:

};


