#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "mazecontroller.h"

#include "model/evaluation.h"

class QGraphicsScene;
class NewMazeDialog;
class SolveMazeDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onNewMazeButtonPressed();

    void onSolveButtonPressed();

    void onEvaluateButtonPressed();

    void onResetButtonPressed();

    void onGenerationFinished();

    void resizeEvent(QResizeEvent *event) override;

private:
    void onEvaluationReportReady(const std::vector<EvaluationReport>& reports);

    Ui::MainWindow *ui;

    NewMazeDialog *newMazeDialog_;

    SolveMazeDialog *solveMazeDialog_;

    std::unique_ptr<MazeController> mazeController_;

    int  *leak;
};

#endif // MAINWINDOW_H
