#include "toycaddata.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <QDebug>
#include <QFile>

using namespace std;

ToyCADData::ToyCADData()
{
    this->scale = 0.0;
}

ToyCADData::~ToyCADData()
{
    this->cad_data.clear();
}

void ToyCADData::loadToyCADData(const std::string& file_path){
    if (!QFile::exists(file_path.c_str())) {
        cerr << "File is not exist @ ToyCADData - " << file_path << endl;
        return;
    }

    ifstream readfile(file_path);

    string line;
    vector<string> tokens;
    string token;
    TOYCAD_DATA cur_data;

    getline(readfile, line); //bbox line
    stringstream ss(line);
    while( getline(ss, token, ',')){
        tokens.push_back(token);
    }
    this->data_bbox.min_x = stof(tokens[TOYCAD_BBOX_IDX_MINX]);
    this->data_bbox.min_y = stof(tokens[TOYCAD_BBOX_IDX_MINY]);
    this->data_bbox.min_z = stof(tokens[TOYCAD_BBOX_IDX_MINZ]);
    this->data_bbox.max_x = stof(tokens[TOYCAD_BBOX_IDX_MAXX]);
    this->data_bbox.max_y = stof(tokens[TOYCAD_BBOX_IDX_MAXY]);
    this->data_bbox.max_z = stof(tokens[TOYCAD_BBOX_IDX_MAXZ]);
    this->calculateScale();

    getline(readfile, line);//column header line
    while(getline(readfile, line)){
        tokens.clear();
        stringstream ss(line);
        while( getline(ss, token, ',')){
            tokens.push_back(token);
        }

        cur_data.type = tokens[TOYCAD_DATA_IDX_TYPE];
        cur_data.layer = tokens[TOYCAD_DATA_IDX_LAYER];
        cur_data.minx = stof(tokens[TOYCAD_DATA_IDX_MINX]);
        cur_data.miny = stof(tokens[TOYCAD_DATA_IDX_MINY]);
        cur_data.maxx = stof(tokens[TOYCAD_DATA_IDX_MAXX]);
        cur_data.maxy = stof(tokens[TOYCAD_DATA_IDX_MAXY]);
        cur_data.minz = stof(tokens[TOYCAD_DATA_IDX_MINZ]);
        cur_data.maxz = stof(tokens[TOYCAD_DATA_IDX_MAXZ]);
        cur_data.color_r = stoi(tokens[TOYCAD_DATA_IDX_COLOR_R]);
        cur_data.color_g = stoi(tokens[TOYCAD_DATA_IDX_COLOR_G]);
        cur_data.color_b = stoi(tokens[TOYCAD_DATA_IDX_COLOR_B]);
        cur_data.color_a = stoi(tokens[TOYCAD_DATA_IDX_COLOR_A]);
        cur_data.slopex = stof(tokens[TOYCAD_DATA_IDX_SLOPEX]);
        cur_data.slopey = stof(tokens[TOYCAD_DATA_IDX_SLOPEY]);
        cur_data.rotation_degree = stof(tokens[TOYCAD_DATA_IDX_ROTATE]);

        this->cad_data.push_back(cur_data);

    }//while(getline(readfile, line)){
    printAllData();
}

void ToyCADData::calculateScale(){
    double diff_x = this->data_bbox.max_x - this->data_bbox.min_x;
    double diff_y = this->data_bbox.max_y - this->data_bbox.min_y;
    double diff_z = this->data_bbox.max_z - this->data_bbox.min_z;

    double diff_max = diff_x > diff_y ? diff_x : diff_y;
    diff_max = diff_max > diff_z ? diff_max : diff_z;

    this->scale = 1.0f / diff_max;
}

void ToyCADData::printAllData(){
    qDebug() << "ToyCADData :: printAllData()";
    for(auto cur : this->cad_data){
        qDebug() << cur.type.c_str() << ", "
                 << cur.layer.c_str() << ", "
                 << cur.minx << ", "
                 << cur.miny << ", "
                 << cur.maxx << ", "
                 << cur.maxy << ", "
                 << cur.minz << ", "
                 << cur.maxz << ", "
                 << cur.color_r << ", "
                 << cur.color_g << ", "
                 << cur.color_b << ", "
                 << cur.slopex << ", "
                 << cur.slopey << ", "
                 << cur.rotation_degree;
    }
}
