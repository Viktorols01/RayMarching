#include <math.h>
#include <utility>

#include "Shape.h"
#include "Cube.h"

Cube::Cube(Vec3 v, double w) : v(v), w(w) {};

double Cube::getMinimumDistance(Vec3 v)
{   
    // My own (possibly flawed) attempt at making a SDF for a cube
    // TODO: rotation
    Vec3 q = (v - this->v).absolute();
    double max = std::max(q.x - w/2, std::max(q.y - w/2, q.z - w/2));
    double min = std::min(q.x - w/2, std::min(q.y - w/2, q.z - w/2));
    return min > 0 ? (q - Vec3(w/2, w/2, w/2)).getLength() : max;
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