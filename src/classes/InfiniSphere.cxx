#include <math.h>

#include "Shape.h"
#include "InfiniSphere.h"

InfiniSphere::InfiniSphere(Vec3 v, double r, double s) : v(v), r(r), s(s) {};

double InfiniSphere::getMinimumDistance(Vec3 v)
{
    return ((v%s - this->v)).getLength() - this->r;
}

Vec3 InfiniSphere::getNormal(Vec3 v) {
    return ((v%s - this->v))/((v%s - this->v)).getLength();
}