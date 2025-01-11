#include "Shape.h"

#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Shape {
    private:
    double x;
    double y;
    double z;
    double r;
    public:
    Sphere::Sphere(double x, double y, double z, double r);
    double getMinimumDistance(double x, double y, double z);
};

#endif