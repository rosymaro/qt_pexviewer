#ifndef T2D_H
#define T2D_H

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

struct bBox{
    double minx;
    double miny;
    double maxx;
    double maxy;
};

struct Color_rgba{
    int r;
    int g;
    int b;
    int a;
};

struct LDATA10by10{
    std::string layername;
    int layernum;
    int datatype;
    Color_rgba color;
    double bot;
    double top;
    std::vector<std::vector<std::vector<bBox>>> xy;
};

struct MinMaxXYZ{
    double minx;
    double maxx;
    double miny;
    double maxy;
    double minz;
    double maxz;
};

class T2D{
    public:
        bool text2data(std::string file_name);

        std::vector<LDATA10by10> LayoutData10by10;
        MinMaxXYZ LayoutMinMax;

        LDATA10by10 buf_ldata10by10;
        vector<vector<vector<bBox>>> buf_xy10by10;
        vector<bBox> buf_xy;
        static std::vector<std::string> string_split(std::string input, char delimiter);
};

#endif // T2D_H
