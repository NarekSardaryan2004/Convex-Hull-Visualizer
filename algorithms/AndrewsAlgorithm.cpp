#include "AndrewsAlgorithm.h"
#include "../geometry/Orientation.h"

#include <algorithm>
#include <vector>

std::vector<Point>
AndrewsAlgorithm::computeHull(const std::vector<Point>& points)
{
    if (points.size() < 3) {
        return points;
    }

    std::vector<Point> pts = points;

    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        if (a.x == b.x) {
            return a.y < b.y;
        }
        return a.x < b.x;
    });

    std::vector<Point> lower;

    for (const Point& p : pts) {
        while (lower.size() >= 2) {
            const Point& p1 = lower[lower.size() - 2];
            const Point& p2 = lower[lower.size() - 1];

            if (orientation(p1, p2, p) == Orientation::CounterClockWise) {
                break;
            }

            lower.pop_back();
        }
        lower.push_back(p);
    }

    std::vector<Point> upper;
    for (int i = static_cast<int>(pts.size()) - 1; i >= 0; --i) {
        const Point& p = pts[i];

        while (upper.size() >= 2) {
            const Point& p1 = upper[upper.size() - 2];
            const Point& p2 = upper[upper.size() - 1];

            if (orientation(p1, p2, p) == Orientation::CounterClockWise) {
                break;
            }

            upper.pop_back();
        }
        upper.push_back(p);
    }

    lower.pop_back();
    upper.pop_back();

    lower.insert(lower.end(), upper.begin(), upper.end());

    return lower;
}

QString AndrewsAlgorithm::name() const
{
    return "Andrew (Monotonic Chain)";
}
