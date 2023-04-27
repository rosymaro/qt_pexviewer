#include "toycadmodel.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#include <QFile>

using namespace std;

ToyCADModel::ToyCADModel(LveDevice& device, MODEL_TYPE model_type)
    : LveModel(device, model_type){
    QString test_input("ToyCAD/Data/toy_cad_test_data.csv");
    num_points_for_circle = 20;
    this->makeRenderingData(test_input);
    //this->createBuffers();
}

ToyCADModel::~ToyCADModel(){

}

void ToyCADModel::makeRenderingData(const QString& file_path){
       this->loadData(file_path.toStdString());

}



void ToyCADModel::makeVertices(){

}

void ToyCADModel::makeIndices(){

}

void ToyCADModel::makeCubeVertices(){}
void ToyCADModel::makeCylinderVertices(){}
void ToyCADModel::makeIndicesForFace(){}

void ToyCADModel::loadData(const string& file_path){
    if (!QFile::exists(file_path.c_str())) {
        cerr << "File is not exist @ ToyCADModel - " << file_path << endl;
        return;
    }

    ifstream readfile(file_path);
    string line;
    vector<string> tokens;
    string token;

    toycad_data cur_data;

    getline(readfile, line); //head line
    while(getline(readfile, line)){
        tokens.clear();
        stringstream ss(line);
        while( getline(ss, token, ',')){
            tokens.push_back(token);
        }

        cur_data.type = tokens[TOYCADINFO_IDX_TYPE];
        cur_data.layer = tokens[TOYCADINFO_IDX_LAYER];
        cur_data.minx = stof(tokens[TOYCADINFO_IDX_XMIN]);
        cur_data.miny = stof(tokens[TOYCADINFO_IDX_YMIN]);
        cur_data.maxx = stof(tokens[TOYCADINFO_IDX_XMAX]);
        cur_data.maxy = stof(tokens[TOYCADINFO_IDX_YMAX]);
        cur_data.minz = stof(tokens[TOYCADINFO_IDX_ZMIN]);
        cur_data.maxz = stof(tokens[TOYCADINFO_IDX_ZMAX]);
        cur_data.slopex = stof(tokens[TOYCADINFO_IDX_XSLOPE]);
        cur_data.slopey = stof(tokens[TOYCADINFO_IDX_YSLOPE]);

        this->cad_datas.push_back(cur_data);

        //qDebug() << "Cur Line : "<< line.c_str();
        //for(auto cur_token : tokens){qDebug() << cur_token.c_str();}
    }


    printAllData();
}

void ToyCADModel::printAllData(){
    qDebug() << "ToyCADModel :: printAllData()";
    for(auto cur : this->cad_datas){
        qDebug() << cur.type.c_str();
        qDebug() << cur.layer.c_str();
        qDebug() << cur.minx;
        qDebug() << cur.miny;
        qDebug() << cur.maxx;
        qDebug() << cur.maxy;
        qDebug() << cur.minz;
        qDebug() << cur.maxz;
        qDebug() << cur.slopex;
        qDebug() << cur.slopey;;
    }
}
