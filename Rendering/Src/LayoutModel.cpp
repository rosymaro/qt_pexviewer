#include "LayoutModel.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "LayoutPEXData.h"
#include <unordered_set>
#include <QFile>


LayoutModel::LayoutModel(LveDevice& device, MODEL_TYPE model_type, T2D &t2d)
    : LveModel(device, model_type), layout_data{} {
    this->makeRenderingData(t2d);
    this->createBuffers();
}

LayoutModel::~LayoutModel() {
    this->layout_data.clear();
}

void LayoutModel::makeRenderingData(T2D &t2d) {
    std::chrono::system_clock::time_point start, end;
    std::chrono::seconds run_time;

    start = std::chrono::system_clock::now();
    loadData(t2d);
    end = std::chrono::system_clock::now();
    run_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Run Time / LayoutModel / loadData [sec] : "
              << run_time.count() << "\n";

    start = std::chrono::system_clock::now();
    makeCubeVertices();
    end = std::chrono::system_clock::now();
    run_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Run Time / LayoutModel / makeCubeVertices [sec] : "
              << run_time.count() << "\n";

    start = std::chrono::system_clock::now();
    makeVertices();
    end = std::chrono::system_clock::now();
    run_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Run Time / LayoutModel / lmakeVertices [sec] : "
              << run_time.count() << "\n";

    start = std::chrono::system_clock::now();
    makeIndices();
    end = std::chrono::system_clock::now();
    run_time = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Run Time / LayoutModel / makeIndices [sec] : "
              << run_time.count() << "\n";
}

void LayoutModel::loadData(T2D &t2d) {
    this->layout_data.loadLayoutData(t2d);
}

void LayoutModel::makeCubeVertices() {
    std::vector<LDATA10BY10> &layout_items = this->layout_data.getPatterns_t2d();
    std::vector<LDATA10BY10>::iterator it = layout_items.begin();
    cube_vertex cur_cube_vertices;
    std::unordered_set<string> checking_set;


    for (it = layout_items.begin(); it != layout_items.end(); ++it) {
        cur_cube_vertices.layernum = to_string(it->layernum) + "." + to_string(it->datatype);
        qDebug() <<  QString::fromStdString(cur_cube_vertices.layernum);
        for(auto row : it->xy ){
            for (auto col : row){
                if(col.size() == 0) continue;
                for(auto each_box : col){

                    cur_cube_vertices.vertex[0] = { each_box.minx, each_box.maxy, it->top };
                    cur_cube_vertices.vertex[1] = { each_box.minx, each_box.miny, it->top };
                    cur_cube_vertices.vertex[2] = { each_box.maxx, each_box.miny, it->top };
                    cur_cube_vertices.vertex[3] = { each_box.maxx, each_box.maxy, it->top };

                    cur_cube_vertices.vertex[4] = { each_box.minx, each_box.maxy, it->bot };
                    cur_cube_vertices.vertex[5] = { each_box.minx, each_box.miny, it->bot };
                    cur_cube_vertices.vertex[6] = { each_box.maxx, each_box.miny, it->bot };
                    cur_cube_vertices.vertex[7] = { each_box.maxx, each_box.maxy, it->bot };


                    this->cube_vertices.push_back(cur_cube_vertices);

                    if(checking_set.find(cur_cube_vertices.layernum) == checking_set.end()){
                        checking_set.insert(cur_cube_vertices.layernum);
                        drawing_order_layerby.push_back(cur_cube_vertices.layernum);
                    }
                }

            }
            layerList[cur_cube_vertices.layernum].color = glm::vec3 {it->color.r/(float)255,it->color.g/(float)255, it->color.b/(float)255};
            layerList[cur_cube_vertices.layernum].opacity = it->color.a/(float)255;
            layerList[cur_cube_vertices.layernum].visiblity = it->checking;

        }


        //std::cout << cube_vertices.back().layernum << '\n';
    }
}

void LayoutModel::makeVertices() {
    std::vector<LDATA10BY10> &layout_items = this->layout_data.getPatterns_t2d();
    Vertex temp_vertex;

    float up_color = 0.15f;
    float down_color = 0.6f;

    for (const auto& cur_cube : this->cube_vertices) {
        for (int i = 0; i < 8; ++i) {
            temp_vertex.position = { cur_cube.vertex[i].x, cur_cube.vertex[i].y, cur_cube.vertex[i].z };

            if (i < 4) temp_vertex.color = { up_color , up_color , up_color };
            else       temp_vertex.color = { down_color , down_color , down_color };


            layerby_vertices[cur_cube.layernum].push_back(temp_vertex);
        }
    }
}

void LayoutModel::makeIndices() {
    //makeIndicesForFace();
    makeIndicesForFace_map();
    //makeIndicesForEdge();
    makeIndicesForEdge_map();
}

void LayoutModel::makeIndicesForFace_map() {
    //size_t layer_count = this->layerby_vertices.size();

    uint32_t start_idx = 0;
    std::map<string, std::vector<uint32_t>>& indices = this->layerby_face;
    for (auto const& [key, val] : layerby_vertices) {
        size_t cube_count_by_layer = val.size() / 8;
        for (size_t i = 0; i < cube_count_by_layer; ++i) {
            start_idx = 8 * i;
            indices[key].push_back(start_idx + 0); indices[key].push_back(start_idx + 1); indices[key].push_back(start_idx + 2);
            indices[key].push_back(start_idx + 2); indices[key].push_back(start_idx + 3); indices[key].push_back(start_idx + 0);//top

            indices[key].push_back(start_idx + 3); indices[key].push_back(start_idx + 2); indices[key].push_back(start_idx + 6);
            indices[key].push_back(start_idx + 6); indices[key].push_back(start_idx + 7); indices[key].push_back(start_idx + 3);//right

            indices[key].push_back(start_idx + 0); indices[key].push_back(start_idx + 3); indices[key].push_back(start_idx + 7);
            indices[key].push_back(start_idx + 7); indices[key].push_back(start_idx + 4); indices[key].push_back(start_idx + 0);//front

            indices[key].push_back(start_idx + 4); indices[key].push_back(start_idx + 7); indices[key].push_back(start_idx + 6);
            indices[key].push_back(start_idx + 6); indices[key].push_back(start_idx + 5); indices[key].push_back(start_idx + 4);//bottom

            indices[key].push_back(start_idx + 0); indices[key].push_back(start_idx + 4); indices[key].push_back(start_idx + 5);
            indices[key].push_back(start_idx + 5); indices[key].push_back(start_idx + 1); indices[key].push_back(start_idx + 0);//left

            indices[key].push_back(start_idx + 1); indices[key].push_back(start_idx + 5); indices[key].push_back(start_idx + 6);
            indices[key].push_back(start_idx + 6); indices[key].push_back(start_idx + 2); indices[key].push_back(start_idx + 1);//back
        }
    }

    qDebug() << layerby_vertices.size();

}

void LayoutModel::makeIndicesForEdge_map() {
    //size_t layer_count = this->layerby_vertices.size();

    std::map<string, std::vector<uint32_t>>& indices = this->layerby_edge;
    uint32_t start_idx = 0;
    for (auto const& [key, val] : layerby_vertices) {
        size_t cube_count_by_layer = val.size() / 8;
        for (size_t i = 0; i < cube_count_by_layer; ++i) {
            start_idx = 8 * i;
            //top
            indices[key].push_back(start_idx + 0); indices[key].push_back(start_idx + 1);
            indices[key].push_back(start_idx + 1); indices[key].push_back(start_idx + 2);
            indices[key].push_back(start_idx + 2); indices[key].push_back(start_idx + 3);
            indices[key].push_back(start_idx + 3); indices[key].push_back(start_idx + 0);

            //bottom
            indices[key].push_back(start_idx + 4); indices[key].push_back(start_idx + 7);
            indices[key].push_back(start_idx + 7); indices[key].push_back(start_idx + 6);
            indices[key].push_back(start_idx + 6); indices[key].push_back(start_idx + 5);
            indices[key].push_back(start_idx + 5); indices[key].push_back(start_idx + 4);

            //right
            indices[key].push_back(start_idx + 3); indices[key].push_back(start_idx + 2);
            indices[key].push_back(start_idx + 2); indices[key].push_back(start_idx + 6);
            indices[key].push_back(start_idx + 6); indices[key].push_back(start_idx + 7);
            indices[key].push_back(start_idx + 7); indices[key].push_back(start_idx + 3);

            //left
            indices[key].push_back(start_idx + 0); indices[key].push_back(start_idx + 4);
            indices[key].push_back(start_idx + 4); indices[key].push_back(start_idx + 5);
            indices[key].push_back(start_idx + 5); indices[key].push_back(start_idx + 1);
            indices[key].push_back(start_idx + 1); indices[key].push_back(start_idx + 0);

            //front
            indices[key].push_back(start_idx + 0); indices[key].push_back(start_idx + 3);
            indices[key].push_back(start_idx + 3); indices[key].push_back(start_idx + 7);
            indices[key].push_back(start_idx + 7); indices[key].push_back(start_idx + 4);
            indices[key].push_back(start_idx + 4); indices[key].push_back(start_idx + 0);

            //back
            indices[key].push_back(start_idx + 1); indices[key].push_back(start_idx + 5);
            indices[key].push_back(start_idx + 5); indices[key].push_back(start_idx + 6);
            indices[key].push_back(start_idx + 6); indices[key].push_back(start_idx + 2);
            indices[key].push_back(start_idx + 2); indices[key].push_back(start_idx + 1);
        }
    }

}

void LayoutModel::makeIndicesForFace() {
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


void LayoutModel::makeIndicesForEdge() {
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


