#ifndef T2D_H
#define T2D_H

#include "defineParam.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

class T2D{
    public:
        bool text2data(std::string file_name);

        std::vector<LDATA10BY10> LayoutData10by10;
        MIN_MAX_XYZ LayoutMinMax;

        LDATA10BY10 buf_ldata10by10;
        vector<vector<vector<B_BOX>>> buf_xy10by10;
        vector<B_BOX> buf_xy;
        static std::vector<std::string> string_split(std::string input, char delimiter);
};

#endif // T2D_H
