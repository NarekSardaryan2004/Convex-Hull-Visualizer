#include "DrawWidget.h"

#include <QPainter>
#include <QFont>

DrawWidget::DrawWidget(AppState* state, QWidget* parent)
    :   QWidget(parent), m_p_state(state)
{
    connect(m_p_state, &AppState::stateChanged, this, &DrawWidget::onStateChanged);
}

void DrawWidget::onStateChanged()
{
    update();
}

void DrawWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const AnimationStep* step = m_p_state->currentStep();

    painter.setPen(QPen(Qt::green, 2));
    const auto& hull = m_p_state->hull();
    if (!hull.empty()) {
        for (size_t i = 0; i < hull.size(); ++i) {
            size_t next = (i + 1) % hull.size();
            painter.drawLine(QPointF(hull[i].x, hull[i].y),
                             QPointF(hull[next].x, hull[next].y));
        }
    }

    if (step && step->type == AnimationStep::HIGHLIGHT_LINE && step->points.size() >= 2) {
        painter.setPen(QPen(QColor(255, 200, 0), 2, Qt::DashLine));
        for (size_t i = 0; i < step->points.size() - 1; ++i) {
            painter.drawLine(QPointF(step->points[i].x, step->points[i].y),
                             QPointF(step->points[i+1].x, step->points[i+1].y));
        }
        if (step->points.size() == 3) {
            painter.drawLine(QPointF(step->points[2].x, step->points[2].y),
                             QPointF(step->points[0].x, step->points[0].y));
        }
    }

    auto contains = [](const std::vector<Point>& vec, const Point& p) {
        for (const Point& pt : vec) {
            if (std::abs(pt.x - p.x) < 0.01 && std::abs(pt.y - p.y) < 0.01) {
                return true;
            }
        }
        return false;
    };

    for (const Point& p : m_p_state->points()) {
        QColor color;
        int size;

        bool isHighlighted = false;
        bool isInHull = contains(hull, p);

        if (step) {
            if (step->type == AnimationStep::HIGHLIGHT_POINT ||
                step->type == AnimationStep::ADD_TO_HULL ||
                step->type == AnimationStep::HIGHLIGHT_LINE) {
                isHighlighted = contains(step->points, p);
            }
            if (step->type == AnimationStep::REMOVE_FROM_HULL) {
                if (contains(step->points, p)) {
                    color = QColor(255, 0, 0);
                    size = 8;
                    painter.setBrush(color);
                    painter.setPen(Qt::NoPen);
                    painter.drawEllipse(QPointF(p.x, p.y), size, size);

                    painter.setPen(QPen(Qt::white, 2));
                    painter.drawLine(QPointF(p.x - size, p.y - size),
                                     QPointF(p.x + size, p.y + size));
                    painter.drawLine(QPointF(p.x - size, p.y + size),
                                     QPointF(p.x + size, p.y - size));
                    continue;
                }
            }
        }

        if (isHighlighted) {
            color = QColor(255, 100, 0);
            size = 8;
        } else if (isInHull) {
            color = QColor(0, 200, 0);
            size = 6;
        } else {
            color = Qt::gray;
            size = 4;
        }

        painter.setBrush(color);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(p.x, p.y), size, size);

        painter.setPen(QPen(Qt::white, 1));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(QPointF(p.x, p.y), size, size);
    }

    if (m_p_state->totalSteps() > 0) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 11, QFont::Bold));

        QString stepInfo = QString("Step: %1 / %2")
                               .arg(m_p_state->currentStepIndex())
                               .arg(m_p_state->totalSteps());
        painter.drawText(10, 20, stepInfo);

        if (step) {
            painter.setFont(QFont("Arial", 10));
            painter.drawText(10, 40, step->description);
        }
    }

    if (m_p_state->finished()) {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 11, QFont::Bold));
        QString timeInfo = QString("Time: %1 s")
                               .arg(m_p_state->elapsedTimeMs(), 0, 'f', 4);
        painter.drawText(10, height() - 10, timeInfo);
    }
}
