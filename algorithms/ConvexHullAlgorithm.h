#ifndef CONVEXHULLALGORITHM_H
#define CONVEXHULLALGORITHM_H

#include <QString>
#include <vector>
#include "../geometry/Point.h"

struct AnimationStep {
    enum Type {
        HIGHLIGHT_POINT,
        HIGHLIGHT_LINE,
        ADD_TO_HULL,
        REMOVE_FROM_HULL,
        FINAL_HULL
    };

    Type type;
    std::vector<Point> points;
    std::vector<Point> currentHull;
    QString description;
};

class ConvexHullAlgorithm
{
public:
    virtual ~ConvexHullAlgorithm() = default;

    virtual std::vector<Point> computeHull(const std::vector<Point>& points) = 0;
    virtual std::vector<AnimationStep> generateSteps(const std::vector<Point>& points) = 0;
    virtual QString name() const = 0;
};

#endif // CONVEXHULLALGORITHM_H
