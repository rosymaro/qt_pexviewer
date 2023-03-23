#include "T2D.h"

using namespace std;

bool T2D::text2data(std::string file_name){
    ifstream readfile(file_name);
    string line;
    vector<string> split;

    getline(readfile, line);
    split = T2D::string_split(line, ' ');
    LayoutMinMax.minx = atof(split[0].c_str());
    LayoutMinMax.miny = atof(split[1].c_str());
    LayoutMinMax.maxx = atof(split[2].c_str());
    LayoutMinMax.maxy = atof(split[3].c_str());
    LayoutMinMax.minz = atof(split[4].c_str());
    LayoutMinMax.maxz = atof(split[5].c_str());
    /*cout << LayoutMinMax.minx << "\t";
    cout << LayoutMinMax.miny << "\t";
    cout << LayoutMinMax.maxx << "\t";
    cout << LayoutMinMax.maxy << "\t";
    cout << LayoutMinMax.minz << "\t";
    cout << LayoutMinMax.maxz << endl;*/
    LayoutData10by10.clear();
    while(getline(readfile, line)){
        split = T2D::string_split(line, ' ');
        int row = atoi(split[9].c_str());
        int col = atoi(split[10].c_str());

        buf_ldata10by10.checking = false;
        buf_ldata10by10.layername = split[0];
        buf_ldata10by10.layernum = atoi(split[1].c_str());
        buf_ldata10by10.datatype = atoi(split[2].c_str());
        buf_ldata10by10.color.r = atoi(split[3].c_str());
        buf_ldata10by10.color.g = atoi(split[4].c_str());
        buf_ldata10by10.color.b = atoi(split[5].c_str());
        buf_ldata10by10.color.a = atoi(split[6].c_str());
        buf_ldata10by10.bot = atof(split[7].c_str());
        buf_ldata10by10.top = atof(split[8].c_str());

        buf_xy10by10.resize(row);
        for(int i = 0 ; i < row ; i++){
            buf_xy10by10[i].resize(col);
        }
        for(int i = 0 ; i < row*col ; i++){
            getline(readfile, line);
            split = T2D::string_split(line, ' ');
            int nr = atoi(split[0].c_str());
            int nc = atoi(split[1].c_str());
            int nsize = atoi(split[2].c_str());

            buf_xy.clear();
            for(int i = 0 ; i < nsize ; i++){
                getline(readfile, line);
                split = T2D::string_split(line, ' ');
                B_BOX buf;
                buf.minx = atof(split[0].c_str());
                buf.miny = atof(split[1].c_str());
                buf.maxx = atof(split[2].c_str());
                buf.maxy = atof(split[3].c_str());
                buf_xy.push_back(buf);
            }
            buf_xy10by10[nr][nc] = buf_xy;
        }
        buf_ldata10by10.xy = buf_xy10by10;
        LayoutData10by10.push_back(buf_ldata10by10);
    }
}

vector<string> T2D::string_split(string input, char delimiter) {
    vector<string> answer;
    stringstream ss(input);
    string temp;

    while(getline(ss, temp, delimiter)) {
        if(temp != ""){
            answer.push_back(temp);
        }
    }

    return answer;
}
