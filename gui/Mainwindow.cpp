#include "Mainwindow.h"

#include <QToolBar>
#include <QAction>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_p_state = new AppState(this);
    m_p_drawWidget = new DrawWidget(m_p_state, this);

    setCentralWidget(m_p_drawWidget);

    QToolBar* p_toolBar = addToolBar("Controls");

    QAction* addPoint = p_toolBar->addAction("Add point");
    QAction* step = p_toolBar->addAction("Step");
    QAction* clear = p_toolBar->addAction("Clear");

    connect(addPoint, &QAction::triggered, this, &MainWindow::onAddPoint);
    connect(step, &QAction::triggered, this, &MainWindow::onStep);
    connect(clear, &QAction::triggered, this, &MainWindow::onClear);
}

void MainWindow::onAddPoint()
{
    Point p;
    p.x = QRandomGenerator::global()->bounded(width());
    p.y = QRandomGenerator::global()->bounded(height());
    m_p_state->addPoint(p);
}

void MainWindow::onStep()
{
    m_p_state->step();
}

void MainWindow::onClear()
{
    m_p_state->clear();
}
