#include <math.h>

#include "Sphere.h"

Sphere::Sphere(double x, double y, double z, double r) : x(x), y(y), z(z), r(r) {};

double Sphere::getMinimumDistance(double x, double y, double z)
{
    return sqrt((x - this->x)*(x - this->x) + (y - this->y)*(y - this->y) + (z - this->z)*(z - this->z)) - this->r;
}
