#include "Sphere.h"

class Sphere : public Shape {
    private:
    double x;
    double y;
    double z;
    double r;
    
    public:
    Sphere::Sphere(double x, double y, double z, double r) : x(x), y(y), z(z), r(r) {};
    double getMinimumDistance(double x, double y, double z) {
        return this->x;
    }
};