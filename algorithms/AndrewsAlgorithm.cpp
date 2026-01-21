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

std::vector<AnimationStep>
AndrewsAlgorithm::generateSteps(const std::vector<Point>& points)
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

    std::vector<Point> pts = points;
    std::sort(pts.begin(), pts.end(), [](const Point& a, const Point& b) {
        if (a.x == b.x) {
            return a.y < b.y;
        }
        return a.x < b.x;
    });

    AnimationStep sortStep;
    sortStep.type = AnimationStep::HIGHLIGHT_POINT;
    sortStep.points = pts;
    sortStep.description = "Sorted points by x-coordinate";
    steps.push_back(sortStep);

    std::vector<Point> lower;
    for (const Point& p : pts) {
        AnimationStep processStep;
        processStep.type = AnimationStep::HIGHLIGHT_POINT;
        processStep.points = {p};
        processStep.currentHull = lower;
        processStep.description = QString("Processing point (%1, %2) for lower hull")
                                      .arg(p.x, 0, 'f', 1).arg(p.y, 0, 'f', 1);
        steps.push_back(processStep);

        while (lower.size() >= 2) {
            const Point& p1 = lower[lower.size() - 2];
            const Point& p2 = lower[lower.size() - 1];

            AnimationStep compStep;
            compStep.type = AnimationStep::HIGHLIGHT_LINE;
            compStep.points = {p1, p2, p};
            compStep.currentHull = lower;
            compStep.description = "Checking orientation";
            steps.push_back(compStep);

            if (orientation(p1, p2, p) == Orientation::CounterClockWise) {
                break;
            }

            AnimationStep removeStep;
            removeStep.type = AnimationStep::REMOVE_FROM_HULL;
            removeStep.points = {p2};
            lower.pop_back();
            removeStep.currentHull = lower;
            removeStep.description = QString("Removed point (%1, %2) - makes clockwise turn")
                                         .arg(p2.x, 0, 'f', 1).arg(p2.y, 0, 'f', 1);
            steps.push_back(removeStep);
        }

        lower.push_back(p);

        AnimationStep addStep;
        addStep.type = AnimationStep::ADD_TO_HULL;
        addStep.points = {p};
        addStep.currentHull = lower;
        addStep.description = QString("Added point (%1, %2) to lower hull")
                                  .arg(p.x, 0, 'f', 1).arg(p.y, 0, 'f', 1);
        steps.push_back(addStep);
    }

    std::vector<Point> upper;
    for (int i = static_cast<int>(pts.size()) - 1; i >= 0; --i) {
        const Point& p = pts[i];

        AnimationStep processStep;
        processStep.type = AnimationStep::HIGHLIGHT_POINT;
        processStep.points = {p};
        std::vector<Point> combined = lower;
        combined.insert(combined.end(), upper.begin(), upper.end());
        processStep.currentHull = combined;
        processStep.description = QString("Processing point (%1, %2) for upper hull")
                                      .arg(p.x, 0, 'f', 1).arg(p.y, 0, 'f', 1);
        steps.push_back(processStep);

        while (upper.size() >= 2) {
            const Point& p1 = upper[upper.size() - 2];
            const Point& p2 = upper[upper.size() - 1];

            AnimationStep compStep;
            compStep.type = AnimationStep::HIGHLIGHT_LINE;
            compStep.points = {p1, p2, p};
            std::vector<Point> combined = lower;
            combined.insert(combined.end(), upper.begin(), upper.end());
            compStep.currentHull = combined;
            compStep.description = "Checking orientation";
            steps.push_back(compStep);

            if (orientation(p1, p2, p) == Orientation::CounterClockWise) {
                break;
            }

            AnimationStep removeStep;
            removeStep.type = AnimationStep::REMOVE_FROM_HULL;
            removeStep.points = {p2};
            upper.pop_back();
            std::vector<Point> combined2 = lower;
            combined2.insert(combined2.end(), upper.begin(), upper.end());
            removeStep.currentHull = combined2;
            removeStep.description = QString("Removed point (%1, %2) - makes clockwise turn")
                                         .arg(p2.x, 0, 'f', 1).arg(p2.y, 0, 'f', 1);
            steps.push_back(removeStep);
        }

        upper.push_back(p);

        AnimationStep addStep;
        addStep.type = AnimationStep::ADD_TO_HULL;
        addStep.points = {p};
        combined = lower;
        combined.insert(combined.end(), upper.begin(), upper.end());
        addStep.currentHull = combined;
        addStep.description = QString("Added point (%1, %2) to upper hull")
                                  .arg(p.x, 0, 'f', 1).arg(p.y, 0, 'f', 1);
        steps.push_back(addStep);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());

    AnimationStep finalStep;
    finalStep.type = AnimationStep::FINAL_HULL;
    finalStep.currentHull = lower;
    finalStep.description = QString("Convex hull complete! %1 points").arg(lower.size());
    steps.push_back(finalStep);

    return steps;
}


QString AndrewsAlgorithm::name() const
{
    return "Andrew (Monotonic Chain)";
}
