#pragma once
#ifndef UTILS
#define UTILS
#endif

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <cstdio>

namespace UTILS {
    inline bool check_file_exsist(const std::string& file_path) {
        FILE* fp = nullptr;
        if (fopen_s(&fp, file_path.c_str(), "r")) {
            fclose(fp);
            return true;
        }
        else {
            return false;
        }
    }

    glm::vec3 getPercentileColour(double v, double vmin, double vmax)
    {
        glm::vec3 c = { 1.0,1.0,0.0 }; // white
        double dv;

        if (v < vmin)
            v = vmin;
        if (v > vmax)
            v = vmax;
        dv = vmax - vmin;

        if (v < (vmin + 0.05 * dv)) {
            c.r = 1;
            c.g = 0;
        }
        else if (v < (vmin + 0.5 * dv)) {
            c.r = 0.5 + 5 * (vmin + 0.5 * dv - v) / dv / 4;
            c.g = 0.5 + 5 * (v - vmin - 0.5 * dv) / dv / 4;
        }
        else if (v < (vmin + 0.95 * dv)) {
            c.r = 0.5 + 5 * (vmin + 0.5 * dv - v) / dv / 4;
            c.g = 0.5 + 5 * (v - vmin - 0.5 * dv) / dv / 4;
        }
        else {
            c.r = 0;
            c.g = 1;
        }


        return(c);
    }
}

