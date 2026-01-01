#ifndef CONVEXHULLALGORITHM_H
#define CONVEXHULLALGORITHM_H

#include <QString>
#include <vector>
#include "../geometry/Point.h"

class ConvexHullAlgorithm
{
public:
    virtual ~ConvexHullAlgorithm() = default;

    virtual std::vector<Point> computeHull(const std::vector<Point>& points) = 0;
    virtual QString name() const = 0;
};

#endif // CONVEXHULLALGORITHM_H
