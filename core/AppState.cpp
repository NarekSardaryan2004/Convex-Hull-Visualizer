#include "AppState.h"
#include "../algorithms/AndrewsAlgorithm.h"
#include "../algorithms/GrahamScan.h"

AppState::AppState(QObject* parent)
    :   QObject(parent),
    m_algorithmType(AlgorithmType::Andrew),
    m_finished(false),
    m_currentStepIndex(0),
    m_isAnimating(false),
    m_animationSpeedMs(500)
{
    createAlgorithm();

    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &AppState::onTimerTick);
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
    m_animationSteps.clear();
    m_currentStepIndex = 0;
    m_finished = false;
    m_isAnimating = false;
    m_animationTimer->stop();
    emit stateChanged();
}

void AppState::setAlgorithm(AlgorithmType type)
{
    m_algorithmType = type;
    createAlgorithm();
    resetAlgorithm();
}

AppState::AlgorithmType AppState::algorithm() const
{
    return m_algorithmType;
}

void AppState::resetAlgorithm()
{
    m_hull.clear();
    m_animationSteps.clear();
    m_currentStepIndex = 0;
    m_finished = false;
    m_isAnimating = false;
    m_animationTimer->stop();
    emit stateChanged();
}

void AppState::step()
{
    if (m_finished || !m_p_algorithm || m_points.size() < 3) {
        return;
    }

    m_elapsedMs.start();
    m_hull = m_p_algorithm->computeHull(m_points);
    m_finished = true;
    emit stateChanged();
}

void AppState::generateAnimationSteps()
{
    if (!m_p_algorithm || m_points.size() < 3) {
        return;
    }

    m_elapsedMs.start();
    m_animationSteps = m_p_algorithm->generateSteps(m_points);
    m_currentStepIndex = 0;
    m_hull.clear();
}

void AppState::startAnimation()
{
    if (m_points.size() < 3) {
        return;
    }

    if (m_animationSteps.empty()) {
        generateAnimationSteps();
    }

    m_isAnimating = true;
    m_animationTimer->start(m_animationSpeedMs);
    emit stateChanged();
}

void AppState::pauseAnimation()
{
    m_isAnimating = false;
    m_animationTimer->stop();
    emit stateChanged();
}

void AppState::stepForward()
{
    if (m_animationSteps.empty()) {
        generateAnimationSteps();
    }

    if (m_currentStepIndex < m_animationSteps.size()) {
        m_currentStepIndex++;
        applyCurrentStep();
        emit stepChanged(m_currentStepIndex, m_animationSteps.size());
        emit stateChanged();

        if (m_currentStepIndex >= m_animationSteps.size()) {
            m_finished = true;
            m_isAnimating = false;
            m_animationTimer->stop();
        }
    }
}

void AppState::stepBackward()
{
    if (m_currentStepIndex > 0) {
        m_currentStepIndex--;
        applyCurrentStep();
        emit stepChanged(m_currentStepIndex, m_animationSteps.size());
        emit stateChanged();

        m_finished = false;
    }
}

void AppState::setAnimationSpeed(int speedMs)
{
    m_animationSpeedMs = speedMs;
    if (m_isAnimating) {
        m_animationTimer->setInterval(speedMs);
    }
}

void AppState::onTimerTick()
{
    stepForward();
}

void AppState::applyCurrentStep()
{
    if (m_currentStepIndex == 0) {
        m_hull.clear();
        return;
    }

    if (m_currentStepIndex > 0 && m_currentStepIndex <= m_animationSteps.size()) {
        const AnimationStep& step = m_animationSteps[m_currentStepIndex - 1];
        m_hull = step.currentHull;
    }
}

const std::vector<Point>& AppState::points() const
{
    return m_points;
}

const std::vector<Point>& AppState::hull() const
{
    return m_hull;
}

const AnimationStep* AppState::currentStep() const
{
    if (m_currentStepIndex > 0 && m_currentStepIndex <= m_animationSteps.size()) {
        return &m_animationSteps[m_currentStepIndex - 1];
    }
    return nullptr;
}

int AppState::currentStepIndex() const
{
    return m_currentStepIndex;
}

int AppState::totalSteps() const
{
    return m_animationSteps.size();
}

bool AppState::isAnimating() const
{
    return m_isAnimating;
}

bool AppState::finished() const
{
    return m_finished;
}

double AppState::elapsedTimeMs() const
{
    return m_elapsedMs.nsecsElapsed() / 1e9;
}

QString AppState::algorithmName() const
{
    return m_p_algorithm ? m_p_algorithm->name() : "None";
}
