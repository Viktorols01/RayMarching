#ifndef VECTOR3_H
#define VECTOR3_H

class Vec3 {
    public:
    double x;
    double y;
    double z;

    Vec3(double x, double y, double z);
    double getLength();
    Vec3 absolute();
    static Vec3 getUnitVector(double phi, double theta);

    Vec3 operator+(Vec3 const& v);
    Vec3 operator-(Vec3 const& v);
    Vec3 operator*(double d);
    Vec3 operator/(double d);
    Vec3 operator%(double d);

    double operator*(Vec3 const& v);
};

#endif