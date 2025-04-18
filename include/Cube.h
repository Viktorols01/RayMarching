#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"
#include "Vec3.h"

class Cube : public Shape {
    private:
    Vec3 v;
    double w;
    public:
    Cube(Vec3 v, double w);
    double getMinimumDistance(Vec3 v);
    Vec3 getNormal(Vec3 v);
};

#endif