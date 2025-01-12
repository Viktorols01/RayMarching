#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {
    private:
    double x;
    double y;
    double z;
    double r;
    public:
    Cube::Cube(double x, double y, double z, double r);
    double getMinimumDistance(double x, double y, double z);
};

#endif