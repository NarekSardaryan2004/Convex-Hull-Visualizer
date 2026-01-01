#include "DrawWidget.h"

#include <QPainter>

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

    painter.setBrush(Qt::gray);
    for (const Point& p : m_p_state->points()) {
        painter.drawEllipse(QPointF(p.x, p.y), 4, 4);
    }

    painter.setPen(QPen(Qt::green, 2));
    const auto& hull = m_p_state->hull();

    if (!hull.empty()) {
        for (size_t i = 0; i < hull.size(); ++i) {
            size_t next = (i + 1) % hull.size();
            painter.drawLine(QPointF(hull[i].x, hull[i].y),
                             QPointF(hull[next].x, hull[next].y));
        }
    }
}
