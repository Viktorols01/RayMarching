#ifndef SHAPE_H
#define SHAPE_H

class Shape {
    public:
    virtual double getMinimumDistance(double x, double y, double z) = 0;
};

#endif