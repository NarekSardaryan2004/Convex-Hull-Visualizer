#include "Mainwindow.h"

#include <QAction>
#include <QMenu>
#include <QStatusBar>
#include <QSpinBox>
#include <QToolBar>
#include <QToolButton>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_p_state = new AppState(this);
    m_p_drawWidget = new DrawWidget(m_p_state, this);

    setCentralWidget(m_p_drawWidget);

    QToolBar* p_toolBar = addToolBar("Controls");

    QSpinBox* p_countBox = new QSpinBox(this);
    p_countBox->setRange(1, 1000000);
    p_countBox->setValue(20);
    p_countBox->setSuffix(" pts");

    p_toolBar->addWidget(p_countBox);

    QAction* addPoint = p_toolBar->addAction("Add point");
    QAction* clear = p_toolBar->addAction("Clear");

    QMenu* stepMenu = new QMenu("Step", this);
    QAction* stepAndrew = stepMenu->addAction("Step (Andrew)");
    QAction* stepGraham = stepMenu->addAction("Step (Graham)");

    QToolButton* stepButton = new QToolButton(this);
    stepButton->setText("Step");
    stepButton->setMenu(stepMenu);
    stepButton->setPopupMode(QToolButton::InstantPopup);

    p_toolBar->addWidget(stepButton);

    connect(addPoint, &QAction::triggered, this, [this, p_countBox](){
        const int count = p_countBox->value();
        const int w = m_p_drawWidget->width();
        const int h = m_p_drawWidget->height();

        if (w <= 0 || h <= 0)
            return;

        for (int i = 0; i < count; ++i) {
            Point p;
            p.x = QRandomGenerator::global()->bounded(w);
            p.y = QRandomGenerator::global()->bounded(h);
            m_p_state->addPoint(p);
        }
    });
    connect(clear, &QAction::triggered, this, &MainWindow::onClear);

    connect(stepAndrew, &QAction::triggered, this, [this]() {
        m_p_state->setAlgorithm(AppState::AlgorithmType::Andrew);
        m_p_state->step();
    });

    connect(stepGraham, &QAction::triggered, this, [this]() {
        m_p_state->setAlgorithm(AppState::AlgorithmType::Graham);
        m_p_state->step();
    });

    connect(m_p_state, &AppState::stateChanged, this, [this]() {
        statusBar()->showMessage(
            "Algorithm: " + m_p_state->algorithmName() +
            " | Time: " + QString::number(m_p_state->elapsedTimeMs(), 'f', 4) + " s"
            );
    });

    statusBar()->showMessage("Ready");
}

void MainWindow::onAddPoint()
{
    Point p;
    p.x = QRandomGenerator::global()->bounded(width());
    p.y = QRandomGenerator::global()->bounded(height());
    m_p_state->addPoint(p);
}

void MainWindow::onClear()
{
    m_p_state->clear();
}
