#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include "../core/AppState.h"

class DrawWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawWidget(AppState* state, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onStateChanged();

private:
    AppState* m_p_state;
};

#endif // DRAWWIDGET_H
