#include <math.h>

#include "Vec3.h"

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) {};

double Vec3::getLength()
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vec3 Vec3::getUnitVector(double phi, double theta)
{
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);
    return Vec3(x, y, z);
}

Vec3 Vec3::operator+(Vec3 const &v)
{
    return Vec3(this->x + v.x, this->y + v.y, this->z + v.z);
}

Vec3 Vec3::operator-(Vec3 const &v)
{
    return Vec3(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vec3 Vec3::operator*(double d)
{
    return Vec3(this->x * d, this->y * d, this->z * d);
}

Vec3 Vec3::operator/(double d)
{
    return Vec3(this->x / d, this->y / d, this->z / d);
}

double Vec3::operator*(Vec3 const& v) {
    return this->x * v.x + this->y * v.y + this->z * v.z;
}