#include <math.h>
#include <utility>

#include "Shape.h"
#include "Cube.h"

Cube::Cube(Vec3 v, double r) : v(v), r(r) {};

double Cube::getMinimumDistance(Vec3 v)
{
    Vec3 q = (v - this->v).absolute();
    double m = std::max(q.x - r, std::max(q.y - r, q.z - r));
    return m > 0 ? m : q.getLength() - r/sqrt(3);
}

Vec3 Cube::getNormal(Vec3 v)
{
    Vec3 q = (v - this->v);

    if (fabs(q.x) > fabs(q.y) && fabs(q.x) > fabs(q.z))
    {
        return Vec3(q.x / fabs(q.x), 0, 0);
    }
    else if (fabs(q.y) > fabs(q.x) && fabs(q.y) > fabs(q.z))
    {
        return Vec3(0, q.y / fabs(q.y), 0);
    }
    else
    {
        return Vec3(0, 0, q.z / fabs(q.z));
    }
}