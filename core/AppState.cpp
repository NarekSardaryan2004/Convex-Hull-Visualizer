#include "AppState.h"

#include "../algorithms/AndrewsAlgorithm.h"
#include "../algorithms/GrahamScan.h"

AppState::AppState(QObject* parent)
    :   QObject(parent), m_algorithmType(AlgorithmType::Andrew), m_finished(false)
{
    createAlgorithm();
}

void AppState::createAlgorithm()
{
    if (m_algorithmType == AlgorithmType::Andrew) {
        m_p_algorithm = std::make_unique<AndrewsAlgorithm>();
    }
    else {
        m_p_algorithm = std::make_unique<GrahamScan>();
    }
}

void AppState::addPoint(const Point& p)
{
    m_points.push_back(p);
    emit stateChanged();
}

void AppState::clear()
{
    m_points.clear();
    m_hull.clear();
    m_finished = false;
    emit stateChanged();
}

void AppState::setAlgorithm(AlgorithmType type)
{
    m_algorithmType = type;
    createAlgorithm();
    resetAlgorithm();
}

void AppState::resetAlgorithm()
{
    m_hull.clear();
    m_finished = false;
    emit stateChanged();
}

void AppState::step()
{
    if (m_finished || !m_p_algorithm || m_points.size() < 3) {
        return;
    }

    m_hull = m_p_algorithm->computeHull(m_points);
    m_finished = true;

    emit stateChanged();
}

const std::vector<Point>& AppState::points() const
{
    return m_points;
}

const std::vector<Point>& AppState::hull() const
{
    return m_hull;
}

bool AppState::finished() const
{
    return m_finished;
}
