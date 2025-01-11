#include "Sphere.h"

Sphere::Sphere(double x, double y, double z, double r) : x(x), y(y), z(z), r(r) {};

double Sphere::getMinimumDistance(double x, double y, double z)
{
    return this->x;
}
