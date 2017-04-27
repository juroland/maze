#ifndef SOLVEMAZEDIALOG_H
#define SOLVEMAZEDIALOG_H

#include <QDialog>

namespace Ui {
class SolveMazeDialog;
}

#include "model/mazesolver.h"

class SolveMazeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolveMazeDialog(QWidget *parent = 0);
    ~SolveMazeDialog();

    int timeStep() const;
    const MazeSolver::ProgramFactory* programFactory() const;

private:
    Ui::SolveMazeDialog *ui;

    std::unique_ptr<MazeSolver::ProgramFactory> dfsProgramFactory_;
    std::unique_ptr<MazeSolver::ProgramFactory> customProgramFactory_;
    std::unique_ptr<MazeSolver::ProgramFactory> randomProgramFactory_;
};

#endif // SOLVEMAZEDIALOG_H
