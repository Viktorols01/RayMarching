#ifndef SHAPE_H
#define SHAPE_H

#include "Vec3.h"

class Shape {
    public:
    virtual double getMinimumDistance(Vec3 v) = 0;
    virtual Vec3 getNormal(Vec3 v) = 0;
    virtual ~Shape() = default;
};

#endif