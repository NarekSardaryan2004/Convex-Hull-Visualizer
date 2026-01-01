#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "Point.h"

enum class Orientation
{
    Collinear,
    ClockWise,
    CounterClockWise
};

inline Orientation orientation(const Point& a, const Point& b, const Point& c)
{
    double value = (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);

    if (value > 0) {
        return Orientation::CounterClockWise;
    }

    if (value < 0) {
        return Orientation::ClockWise;
    }

    return Orientation::Collinear;
}

#endif // ORIENTATION_H
