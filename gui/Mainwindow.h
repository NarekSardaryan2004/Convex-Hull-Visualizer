#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../core/AppState.h"
#include "../gui/DrawWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onAddPoint();
    void onStep();
    void onClear();

private:
    AppState* m_p_state;
    DrawWidget* m_p_drawWidget;
};
#endif // MAINWINDOW_H
