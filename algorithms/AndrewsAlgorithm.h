#ifndef ANDREWSALGORITHM_H
#define ANDREWSALGORITHM_H

#include "ConvexHullAlgorithm.h"

class AndrewsAlgorithm : public ConvexHullAlgorithm
{
public:
    AndrewsAlgorithm() = default;
    ~AndrewsAlgorithm() override = default;

    std::vector<Point> computeHull(const std::vector<Point>& points) override;
    QString name() const override;

private:
    std::vector<Point> buildHalf(const std::vector<Point>& points);
};

#endif // ANDREWSALGORITHM_H
