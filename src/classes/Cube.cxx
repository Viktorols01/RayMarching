#include <math.h>

#include "Shape.h"
#include "Cube.h"
#include <utility>

Cube::Cube(Vec3 v, double r) : v(v), r(r) {};

double Cube::getMinimumDistance(Vec3 v)
{   
    
    return 0;
}

Vec3 Cube::getNormal(Vec3 v) {
    return v - this->v;
}