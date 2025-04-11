#ifndef COMBINEDSHAPE_H
#define COMBINEDSHAPE_H

#include "Vec3.h"
#include "Shape.h"

class CombinedShape : public Shape {
    private:
    Shape* s1;
    Shape* s2;
    bool subtract;
    public:
    CombinedShape(Shape* s1, Shape* s2, bool subtract);
    double getMinimumDistance(Vec3 v);
    Vec3 getNormal(Vec3 v);
};

#endif