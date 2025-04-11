#ifndef INFINISPHERE_H
#define INFINISPHERE_H

#include "Vec3.h"
#include "Shape.h"

class InfiniSphere : public Shape {
    private:
    Vec3 v;
    double r;
    double s;
    public:
    InfiniSphere(Vec3 v, double r, double s);
    double getMinimumDistance(Vec3 v);
    Vec3 getNormal(Vec3 v);
};

#endif