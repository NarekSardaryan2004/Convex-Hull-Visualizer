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

std::vector<AnimationStep>
GrahamScan::generateSteps(const std::vector<Point>& points)
{
    std::vector<AnimationStep> steps;

    if (points.size() < 3) {
        AnimationStep finalStep;
        finalStep.type = AnimationStep::FINAL_HULL;
        finalStep.currentHull = points;
        finalStep.description = "Not enough points for hull";
        steps.push_back(finalStep);
        return steps;
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

    AnimationStep pivotStep;
    pivotStep.type = AnimationStep::HIGHLIGHT_POINT;
    pivotStep.points = {pivot};
    pivotStep.description = QString("Found pivot (lowest point): (%1, %2)")
                                .arg(pivot.x, 0, 'f', 1).arg(pivot.y, 0, 'f', 1);
    steps.push_back(pivotStep);

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

    AnimationStep sortStep;
    sortStep.type = AnimationStep::HIGHLIGHT_POINT;
    sortStep.points = pts;
    sortStep.description = "Sorted points by polar angle from pivot";
    steps.push_back(sortStep);

    std::vector<Point> hull;
    hull.push_back(pivot);
    hull.push_back(pts[0]);
    hull.push_back(pts[1]);

    AnimationStep initStep;
    initStep.type = AnimationStep::ADD_TO_HULL;
    initStep.points = {pivot, pts[0], pts[1]};
    initStep.currentHull = hull;
    initStep.description = "Initialize hull with pivot and first 2 points";
    steps.push_back(initStep);

    for (int i = 2; i < pts.size(); ++i) {
        const Point& currentPoint = pts[i];

        AnimationStep processStep;
        processStep.type = AnimationStep::HIGHLIGHT_POINT;
        processStep.points = {currentPoint};
        processStep.currentHull = hull;
        processStep.description = QString("Processing point (%1, %2)")
                                      .arg(currentPoint.x, 0, 'f', 1).arg(currentPoint.y, 0, 'f', 1);
        steps.push_back(processStep);

        while (hull.size() >= 2) {
            const Point& p1 = hull[hull.size() - 2];
            const Point& p2 = hull[hull.size() - 1];
            const Point& p3 = currentPoint;

            // Show comparison triangle
            AnimationStep checkStep;
            checkStep.type = AnimationStep::HIGHLIGHT_LINE;
            checkStep.points = {p1, p2, p3};
            checkStep.currentHull = hull;
            checkStep.description = "Checking if points make counter-clockwise turn";
            steps.push_back(checkStep);

            if (orientation(p1, p2, p3) == Orientation::CounterClockWise) {
                break;
            }

            AnimationStep removeStep;
            removeStep.type = AnimationStep::REMOVE_FROM_HULL;
            removeStep.points = {p2};
            hull.pop_back();
            removeStep.currentHull = hull;
            removeStep.description = QString("Removed point (%1, %2) - makes clockwise turn")
                                         .arg(p2.x, 0, 'f', 1).arg(p2.y, 0, 'f', 1);
            steps.push_back(removeStep);
        }

        hull.push_back(currentPoint);

        AnimationStep addStep;
        addStep.type = AnimationStep::ADD_TO_HULL;
        addStep.points = {currentPoint};
        addStep.currentHull = hull;
        addStep.description = QString("Added point (%1, %2) to hull")
                                  .arg(currentPoint.x, 0, 'f', 1).arg(currentPoint.y, 0, 'f', 1);
        steps.push_back(addStep);
    }

    AnimationStep finalStep;
    finalStep.type = AnimationStep::FINAL_HULL;
    finalStep.currentHull = hull;
    finalStep.description = QString("Graham Scan complete! %1 points").arg(hull.size());
    steps.push_back(finalStep);

    return steps;
}

QString GrahamScan::name() const
{
    return "Graham Scan";
}
