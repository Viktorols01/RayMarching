#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"
#include "Shape.h"

class Sphere : public Shape {
    private:
    Vec3 v;
    double r;
    public:
    Sphere::Sphere(Vec3 v, double r);
    double getMinimumDistance(Vec3 v);
    Vec3 getNormal(Vec3 v);
};

#endif