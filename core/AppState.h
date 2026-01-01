#ifndef APPSTATE_H
#define APPSTATE_H

#include <QObject>
#include <vector>
#include <memory>

#include "../geometry/Point.h"
#include "../algorithms/ConvexHullAlgorithm.h"

class AppState : public QObject
{
    Q_OBJECT

public:
    enum class AlgorithmType {
        Andrew,
        Graham
    };

    explicit AppState(QObject* parent = nullptr);

    void addPoint(const Point& p);
    void clear();

    void setAlgorithm(AlgorithmType type);
    void resetAlgorithm();
    void step();

    const std::vector<Point>& points() const;
    const std::vector<Point>& hull() const;
    bool finished() const;

signals:
    void stateChanged();

private:
    void createAlgorithm();

private:
    std::vector<Point> m_points;
    std::vector<Point> m_hull;

    AlgorithmType m_algorithmType;
    std::unique_ptr<ConvexHullAlgorithm> m_p_algorithm;

    bool m_finished;
};

#endif // APPSTATE_H
