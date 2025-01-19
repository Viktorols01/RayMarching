#include <math.h>
#include <utility>
#include <iostream>

#include "Shape.h"
#include "CombinedShape.h"

CombinedShape::CombinedShape(Shape *s1, Shape *s2, bool subtract) : s1(s1), s2(s2), subtract(subtract) {};

double CombinedShape::getMinimumDistance(Vec3 v)
{
    if (subtract)
    {
        return std::max(s1->getMinimumDistance(v), -s2->getMinimumDistance(v));
    }
    else
    {
        return std::min(s1->getMinimumDistance(v), s2->getMinimumDistance(v));
    }
}

Vec3 CombinedShape::getNormal(Vec3 v)
{
    if (subtract)
    {
        return s1->getMinimumDistance(v) > -(s2->getMinimumDistance(v)) ? s1->getNormal(v) : s2->getNormal(v) * -1.0;
    }
    else
    {
        return s1->getMinimumDistance(v) < s2->getMinimumDistance(v) ? s1->getNormal(v) : s2->getNormal(v);
    }
}