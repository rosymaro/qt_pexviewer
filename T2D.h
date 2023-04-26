#ifndef T2D_H
#define T2D_H

#include "defineParam.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

using namespace std;

struct Instance{
    int level;
    string name;
    string num;
    B_BOX box;
};

class T2D{
    public:
        bool text2data(std::string file_name);

        std::vector<LDATA10BY10> LayoutData10by10;
        map<string, LDATA10BY10> map_LayoutData10by10_;
        MIN_MAX_XYZ LayoutMinMax;

        LDATA10BY10 buf_ldata10by10;
        vector<vector<vector<B_BOX>>> buf_xy10by10;
        vector<B_BOX> buf_xy;
        static std::vector<std::string> string_split(std::string input, char delimiter);

        vector<Instance> HierarchyInstance;

        int row;
        int col;

        bool m_filter_onoff = false;
};

#endif // T2D_H
