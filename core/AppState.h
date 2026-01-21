#ifndef APPSTATE_H
#define APPSTATE_H

#include <QObject>
#include <QElapsedTimer>
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
    AlgorithmType algorithm() const;
    void resetAlgorithm();
    void step();

    const std::vector<Point>& points() const;
    const std::vector<Point>& hull() const;
    bool finished() const;

    double elapsedTimeMs() const;
    QString algorithmName() const;

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
    QElapsedTimer m_elapsedMs;
};

#endif // APPSTATE_H
