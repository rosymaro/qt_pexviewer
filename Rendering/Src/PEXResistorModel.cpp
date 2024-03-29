#include "PEXResistorModel.h"

#include <iostream>
#include <string>
#include <vector>

#include "LayoutPEXData.h"
#include "utils.h"

#include <QFile>
#include <QString>
#include <QDebug>

PEXResistorModel::PEXResistorModel(LveDevice& device, MODEL_TYPE model_type, const QString& data_file)
    : LveModel(device, model_type), pex_data{} {
    this->res_cube_thickness = 0.003;
    this->makeRenderingData(data_file);
    this->createBuffers();
}

PEXResistorModel::~PEXResistorModel() {
    this->pex_data.clear();
}

void PEXResistorModel::makeRenderingData(const QString& file_path) {
    if (!QFile::exists(file_path)) {
        qDebug() << "Error! " << file_path << " is not exist!!\n";
        return;
    }

    loadData(file_path);
    groupResistorOfNode();
    makeCubes();
    makeCubeVertices();
    makeVertices();
    makeIndices();
    makeIndicesForEdge_map();

    printf("\nPEX Resistor\n");
    printf("\tresistor count      : %lld\n", this->pex_data.getResistors().size());
    printf("\tcube_info count     : %lld\n", this->cube_infos.size());
    printf("\tcube_vertices count : %lld\n", this->cube_vertices.size());
    printf("\tVertices count      : %lld\n", this->vertices.size());
    printf("\tIndices count       : %lld\n", this->indices_face.size());

}

void PEXResistorModel::loadData(const QString& file_path) {
    this->pex_data.loadData(file_path);
    //this->pex_data.printData();
}

void PEXResistorModel::groupResistorOfNode() {
    this->pex_data.groupResistorOfNode();
    //this->pex_data.printData();
}

void PEXResistorModel::setDirectionOfResistor(PEXResDirection _direction) {
    this->pex_data.setDriectionOfResistor(_direction);
    //this->pex_data.printData();
}


void PEXResistorModel::increaseNumOfResistorGroup() {
    this->pex_data.increaseNumOfResistorGroup();
    //this->pex_data.printData();
}


void PEXResistorModel::decreaseNumOfResistorGroup() {
    this->pex_data.decreaseNumOfResistorGroup();
    //this->pex_data.printData();
}



void PEXResistorModel::makeCubes() {
    std::vector<PEXResistor>& resistors = this->pex_data.getResistors();
    cube_info cur_cube{};
    for (auto cur_res : resistors) {
        this->makeCube(cur_res, cur_cube, cur_res.getDirection());
        this->cube_infos.push_back(cur_cube);
    }
}

void PEXResistorModel::makeCube(const PEXResistor& res, cube_info& cube, PEXResDirection res_direction) {
    if (res_direction == RES_DIRECTION_VERTICAL) {
        cube.minx = res.node1.x - this->res_cube_thickness;
        cube.maxx = res.node1.x + this->res_cube_thickness;
        cube.miny = res.node1.y - this->res_cube_thickness;
        cube.maxy = res.node1.y + this->res_cube_thickness;
        cube.minz = (res.node1.z_end + res.node1.z_start) / 2;
        cube.maxz = (res.node2.z_end + res.node2.z_start) / 2;
    }
    else if (res_direction == RES_DIRECTION_HORIZONTAL) {
        if (res.node1.x == res.node2.x) {//run-length direction : y
            cube.minx = res.node1.x - this->res_cube_thickness;
            cube.maxx = res.node1.x + this->res_cube_thickness;
            cube.miny = res.node1.y < res.node2.y ? res.node1.y : res.node2.y;
            cube.maxy = res.node1.y < res.node2.y ? res.node2.y : res.node1.y;
        }
        else if (res.node1.y == res.node2.y) {//run-length direction : x
            cube.minx = res.node1.x < res.node2.x ? res.node1.x : res.node2.x;
            cube.maxx = res.node1.x < res.node2.x ? res.node2.x : res.node1.x;
            cube.miny = res.node1.y - this->res_cube_thickness;
            cube.maxy = res.node1.y + this->res_cube_thickness;
        }


        cube.minz = res.node1.z_end;
        cube.maxz = res.node2.z_start;
        /*

                    double midz = (res.node1.z_start + res.node1.z_end) * 0.5;
                    cube.minz = midz - this->res_cube_thickness;
                    cube.maxz = midz + this->res_cube_thickness;
        */
    }
    else {
        std::cerr << "\nResistor has no direction\n";
    }
}


void PEXResistorModel::makeCubeVertices() {
    std::vector<cube_info>::const_iterator it;
    cube_vertex cur_cube_vertices;

    for (it = this->cube_infos.begin(); it != this->cube_infos.end(); ++it) {
        cur_cube_vertices.vertex[0] = { it->minx, it->maxy, it->maxz };
        cur_cube_vertices.vertex[1] = { it->minx, it->miny, it->maxz };
        cur_cube_vertices.vertex[2] = { it->maxx, it->miny, it->maxz };
        cur_cube_vertices.vertex[3] = { it->maxx, it->maxy, it->maxz };

        cur_cube_vertices.vertex[4] = { it->minx, it->maxy, it->minz };
        cur_cube_vertices.vertex[5] = { it->minx, it->miny, it->minz };
        cur_cube_vertices.vertex[6] = { it->maxx, it->miny, it->minz };
        cur_cube_vertices.vertex[7] = { it->maxx, it->maxy, it->minz };

        this->cube_vertices.push_back(cur_cube_vertices);
    }
}

void PEXResistorModel::makeVertices() {
    VertexResistor temp_vertex;

    double max_vertical_res = this->pex_data.getMaxResistorVerticalValue();
    //double min_vertical_res = this->pex_data.getMinResistorVerticalValue();
    double max_horizontal_res = this->pex_data.getMaxResistorHorizontalValue();
    //double min_horizontal_res = this->pex_data.getMinResistorHorizontalValue();
    double max_res = max_vertical_res > max_horizontal_res ? max_vertical_res : max_horizontal_res;

    std::vector<PEXResistor>::iterator cur_resistor = this->pex_data.getResistors().begin();
    double resistor_value{};
    double resistor_percentile{};
    PEXResDirection resistor_direction{};
    float temp_color[3] = { 0.0f, 0.0f, 0.0f };
    glm::vec3 resistor_color;
    for (const auto& cur_cube : this->cube_vertices) {
        resistor_value = cur_resistor->getValue();
        resistor_direction = cur_resistor->getDirection();
        resistor_percentile = percentileOfResistance(*cur_resistor);
        resistor_color = UTILS::getPercentileColour(resistor_percentile / 100, 0.0, 1.0);
        //std::cout << ", " << resistor_value << ", " << resistor_percentile / 100 << "," << resistor_color.r << "," << resistor_color.g << "," << resistor_color.b << "," << std::endl;

        for (int i = 0; i < 8; ++i) {
            temp_vertex.position = {
                cur_cube.vertex[i].x, cur_cube.vertex[i].y, cur_cube.vertex[i].z };
            //if (resistor_direction == PEXResDirection::RES_DIRECTION_VERTICAL) {
            //temp_vertex.color = { resistor_percentile / 100, 0.0f, 1-resistor_percentile/100 };
            temp_vertex.color = { resistor_color,0.2 };
            //	}
                //else {
                    //temp_vertex.color = { 0.0f, 0.0f, static_cast<float>(resistor_value / max_res) };
                //}
            //temp_vertex.color_vert_alpha = { 0.2,1.0,1.0 };
            vertices_resistor.push_back(temp_vertex);
        }//for i 0 to 8
        cur_resistor++;
    }//for auto cur_vertices : this->cubes_vertices
}


void PEXResistorModel::makeIndices() {
    size_t cube_count = this->cube_vertices.size();

    uint32_t start_idx = 0;
    std::vector<uint32_t>& indices = this->indices_face;
    for (int i = 0; i < cube_count; ++i) {
        start_idx = 8 * i;
        indices.push_back(start_idx + 0); indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
        indices.push_back(start_idx + 2); indices.push_back(start_idx + 3); indices.push_back(start_idx + 0);//top

        indices.push_back(start_idx + 3); indices.push_back(start_idx + 2); indices.push_back(start_idx + 6);
        indices.push_back(start_idx + 6); indices.push_back(start_idx + 7); indices.push_back(start_idx + 3);//right

        indices.push_back(start_idx + 0); indices.push_back(start_idx + 3); indices.push_back(start_idx + 7);
        indices.push_back(start_idx + 7); indices.push_back(start_idx + 4); indices.push_back(start_idx + 0);//front

        indices.push_back(start_idx + 4); indices.push_back(start_idx + 7); indices.push_back(start_idx + 6);
        indices.push_back(start_idx + 6); indices.push_back(start_idx + 5); indices.push_back(start_idx + 4);//bottom

        indices.push_back(start_idx + 0); indices.push_back(start_idx + 4); indices.push_back(start_idx + 5);
        indices.push_back(start_idx + 5); indices.push_back(start_idx + 1); indices.push_back(start_idx + 0);//left

        indices.push_back(start_idx + 1); indices.push_back(start_idx + 5); indices.push_back(start_idx + 6);
        indices.push_back(start_idx + 6); indices.push_back(start_idx + 2); indices.push_back(start_idx + 1);//back
    }
}

void PEXResistorModel::makeIndicesForEdge_map() {
    size_t cube_count = this->cube_vertices.size();

    std::vector<uint32_t>& indices = this->indices_edge;
    uint32_t start_idx = 0;
    for (int i = 0; i < cube_count; ++i, start_idx += 8) {
        //top
        indices.push_back(start_idx + 0); indices.push_back(start_idx + 1);
        indices.push_back(start_idx + 1); indices.push_back(start_idx + 2);
        indices.push_back(start_idx + 2); indices.push_back(start_idx + 3);
        indices.push_back(start_idx + 3); indices.push_back(start_idx + 0);

        //bottom
        indices.push_back(start_idx + 4); indices.push_back(start_idx + 7);
        indices.push_back(start_idx + 7); indices.push_back(start_idx + 6);
        indices.push_back(start_idx + 6); indices.push_back(start_idx + 5);
        indices.push_back(start_idx + 5); indices.push_back(start_idx + 4);

        //right
        indices.push_back(start_idx + 3); indices.push_back(start_idx + 2);
        indices.push_back(start_idx + 2); indices.push_back(start_idx + 6);
        indices.push_back(start_idx + 6); indices.push_back(start_idx + 7);
        indices.push_back(start_idx + 7); indices.push_back(start_idx + 3);

        //left
        indices.push_back(start_idx + 0); indices.push_back(start_idx + 4);
        indices.push_back(start_idx + 4); indices.push_back(start_idx + 5);
        indices.push_back(start_idx + 5); indices.push_back(start_idx + 1);
        indices.push_back(start_idx + 1); indices.push_back(start_idx + 0);

        //front
        indices.push_back(start_idx + 0); indices.push_back(start_idx + 3);
        indices.push_back(start_idx + 3); indices.push_back(start_idx + 7);
        indices.push_back(start_idx + 7); indices.push_back(start_idx + 4);
        indices.push_back(start_idx + 4); indices.push_back(start_idx + 0);

        //back
        indices.push_back(start_idx + 1); indices.push_back(start_idx + 5);
        indices.push_back(start_idx + 5); indices.push_back(start_idx + 6);
        indices.push_back(start_idx + 6); indices.push_back(start_idx + 2);
        indices.push_back(start_idx + 2); indices.push_back(start_idx + 1);
    }
}


double PEXResistorModel::percentileOfResistance(const PEXResistor& in_res) {
    std::vector<PEXResistor>::const_iterator it;
    int count = 0;
    double percent;
    for (it = this->pex_data.getResistors().begin(); it != this->pex_data.getResistors().end(); ++it) {
        if (it->value > in_res.value) {
            count++;
        }
    }
    percent = (count * 100) / this->pex_data.getResistors().size();
    return percent;
};





