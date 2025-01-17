#include <math.h>

#include "Shape.h"
#include "Sphere.h"

Sphere::Sphere(Vec3 v, double r) : v(v), r(r) {};

double Sphere::getMinimumDistance(Vec3 v)
{
    return (v - this->v).getLength() - this->r;
}

Vec3 Sphere::getNormal(Vec3 v) {
    return (v - this->v)/(v - this->v).getLength();
}