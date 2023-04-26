#ifndef DEFINETYPE_H
#define DEFINETYPE_H

#include <iostream>
#include <vector>

using namespace std;

struct POS_MONITORING{
    double x;
    double y;
    double z;
    double tilt;
    double rotation;
    double zoom;
    double window_zoom;
};

struct POS_SET
{
    std::string name;
    POS_MONITORING position;
};

struct B_BOX{
    double minx;
    double miny;
    double maxx;
    double maxy;
};

struct COLOR_RGBA{
    int r;
    int g;
    int b;
    int a;
};

struct LDATA10BY10{
    bool checking;
    std::string layername;
    int layernum;
    int datatype;
    COLOR_RGBA color;
    double bot;
    double top;
    std::vector<std::vector<std::vector<B_BOX>>> xy;
    std::vector<std::vector<std::vector<B_BOX>>> xy_filtered;
};

struct MIN_MAX_XYZ{
    double minx;
    double maxx;
    double miny;
    double maxy;
    double minz;
    double maxz;
};


#endif // DEFINETYPE_H


