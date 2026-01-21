#include "Mainwindow.h"

#include <QAction>
#include <QMenu>
#include <QStatusBar>
#include <QSpinBox>
#include <QToolBar>
#include <QToolButton>
#include <QSlider>
#include <QLabel>
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

    QAction* addPoint = p_toolBar->addAction("Add points");
    QAction* clear = p_toolBar->addAction("Clear");

    p_toolBar->addSeparator();

    QMenu* algoMenu = new QMenu("Algorithm", this);
    QAction* selectAndrew = algoMenu->addAction("Andrew (Monotone Chain)");
    QAction* selectGraham = algoMenu->addAction("Graham Scan");

    QToolButton* algoButton = new QToolButton(this);
    algoButton->setText("Algorithm");
    algoButton->setMenu(algoMenu);
    algoButton->setPopupMode(QToolButton::InstantPopup);
    p_toolBar->addWidget(algoButton);

    QMenu* stepMenu = new QMenu("Step", this);
    QAction* stepAndrew = stepMenu->addAction("Instant (Andrew)");
    QAction* stepGraham = stepMenu->addAction("Instant (Graham)");

    QToolButton* stepButton = new QToolButton(this);
    stepButton->setText("Instant");
    stepButton->setMenu(stepMenu);
    stepButton->setPopupMode(QToolButton::InstantPopup);
    p_toolBar->addWidget(stepButton);

    p_toolBar->addSeparator();

    QAction* playAction = p_toolBar->addAction("▶ Play");
    QAction* pauseAction = p_toolBar->addAction("⏸ Pause");
    QAction* stepForwardAction = p_toolBar->addAction("→ Step");
    QAction* stepBackAction = p_toolBar->addAction("← Back");

    p_toolBar->addSeparator();

    QLabel* speedLabel = new QLabel(" Speed: ", this);
    p_toolBar->addWidget(speedLabel);

    QSlider* speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(100, 2000);
    speedSlider->setValue(500);
    speedSlider->setMaximumWidth(150);
    speedSlider->setToolTip("Animation speed (slower ← → faster)");
    p_toolBar->addWidget(speedSlider);

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

    connect(selectAndrew, &QAction::triggered, this, [this]() {
        m_p_state->setAlgorithm(AppState::AlgorithmType::Andrew);
    });

    connect(selectGraham, &QAction::triggered, this, [this]() {
        m_p_state->setAlgorithm(AppState::AlgorithmType::Graham);
    });

    connect(stepAndrew, &QAction::triggered, this, [this]() {
        m_p_state->setAlgorithm(AppState::AlgorithmType::Andrew);
        m_p_state->step();
    });

    connect(stepGraham, &QAction::triggered, this, [this]() {
        m_p_state->setAlgorithm(AppState::AlgorithmType::Graham);
        m_p_state->step();
    });

    connect(playAction, &QAction::triggered, this, [this]() {
        m_p_state->startAnimation();
    });

    connect(pauseAction, &QAction::triggered, this, [this]() {
        m_p_state->pauseAnimation();
    });

    connect(stepForwardAction, &QAction::triggered, this, [this]() {
        m_p_state->stepForward();
    });

    connect(stepBackAction, &QAction::triggered, this, [this]() {
        m_p_state->stepBackward();
    });

    connect(speedSlider, &QSlider::valueChanged, this, [this](int value) {
        int speed = 2100 - value;
        m_p_state->setAnimationSpeed(speed);
    });
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
