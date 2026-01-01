#include "GrahamScan.h"
#include "../geometry/Orientation.h"

#include <algorithm>
#include <cmath>

std::vector<Point>
GrahamScan::computeHull(const std::vector<Point>& points)
{
    if (points.size() < 3) {
        return points;
    }

    int pivotIndex = 0;
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].y < points[pivotIndex].y ||
            (points[i].y == points[pivotIndex].y &&
            points[i].x < points[pivotIndex].x))
        {
            pivotIndex = i;
        }
    }

    Point pivot = points[pivotIndex];

    std::vector<Point> pts;
    for (int i = 0; i < points.size(); ++i) {
        if (i != pivotIndex) {
            pts.push_back(points[i]);
        }
    }

    std::sort(pts.begin(), pts.end(),
              [&pivot](const Point& a, const Point& b){

        Orientation o = orientation(pivot, a, b);
        if (o == Orientation::Collinear) {
            double da =
                (a.x - pivot.x) * (a.x - pivot.x) +
                (a.y - pivot.y) * (a.y - pivot.y);
            double db =
                (b.x - pivot.x) * (b.x - pivot.x) +
                (b.y - pivot.y) * (b.y - pivot.y);

            return da < db;
        }

        return o == Orientation::CounterClockWise;
    });

    std::vector<Point> hull;
    hull.push_back(pivot);
    hull.push_back(pts[0]);
    hull.push_back(pts[1]);

    for (int i = 2; i < pts.size(); ++i) {
        while (hull.size() >= 2) {
            const Point& p1 = hull[hull.size() - 2];
            const Point& p2 = hull[hull.size() - 1];
            const Point& p3 = pts[i];

            if (orientation(p1, p2, p3) == Orientation::CounterClockWise) {
                break;
            }

            hull.pop_back();
        }
        hull.push_back(pts[i]);
    }

    return hull;
}

QString GrahamScan::name() const
{
    return "Graham Scan";
}
