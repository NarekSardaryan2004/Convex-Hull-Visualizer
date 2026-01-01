#ifndef GRAHAMSCAN_H
#define GRAHAMSCAN_H

#include "ConvexHullAlgorithm.h"

class GrahamScan : public ConvexHullAlgorithm
{
public:
    GrahamScan() = default;
    ~GrahamScan() override = default;

    std::vector<Point> computeHull(const std::vector<Point>& points) override;
    QString name() const override;
};

#endif // GRAHAMSCAN_H
