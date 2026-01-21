#ifndef APPSTATE_H
#define APPSTATE_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>
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

    void startAnimation();
    void pauseAnimation();
    void stepForward();
    void stepBackward();
    void setAnimationSpeed(int speedMs);

    const AnimationStep* currentStep() const;
    int currentStepIndex() const;
    int totalSteps() const;
    bool isAnimating() const;

    const std::vector<Point>& points() const;
    const std::vector<Point>& hull() const;
    bool finished() const;

    double elapsedTimeMs() const;
    QString algorithmName() const;

signals:
    void stateChanged();
    void stepChanged(int current, int total);

private slots:
    void onTimerTick();

private:
    void createAlgorithm();
    void generateAnimationSteps();
    void applyCurrentStep();

private:
    std::vector<Point> m_points;
    std::vector<Point> m_hull;

    AlgorithmType m_algorithmType;
    std::unique_ptr<ConvexHullAlgorithm> m_p_algorithm;

    bool m_finished;
    QElapsedTimer m_elapsedMs;

    std::vector<AnimationStep> m_animationSteps;
    int m_currentStepIndex;
    bool m_isAnimating;
    QTimer* m_animationTimer;
    int m_animationSpeedMs;
};

#endif // APPSTATE_H
