#ifndef MAZECONTROLLER_H
#define MAZECONTROLLER_H

#include <QTimer>
#include <memory>
#include <chrono>
#include <QFuture>
#include <QFutureWatcher>

#include "model/maze.h"
#include "model/mazesolver.h"
#include "model/algorithm.h"

#include "mazescene.h"

class MazeView;
class MazeSolverView;

class MazeController : public QObject
{
    Q_OBJECT
public:
    MazeController(MazeView *view);

    void newMaze(size_t nrow,
                 size_t ncol,
                 const MazeGeneratorFactory* factory,
                 int timeStep = 0);
    void onMazeInitFinished();
    void onMazeGenerationStep();

    void solve(const MazeSolver::ProgramFactory*, int timeStep);
    void onMazeSolveStep();
    void resetSolve();

    void fitMazeInView();

signals:
    void generationFinished();

private:
    std::unique_ptr<Maze> maze_;

    MazeView* view_;
    std::unique_ptr<MazeScene> mazeScene_;

    std::unique_ptr<MazeSolver> mazeSolver_;
    QTimer solveTimer_;

    std::unique_ptr<MazeGenerator> mazeGenerator_;
    QTimer generationTimer_;

    QFuture<std::pair<Maze*, MazeGenerator*>> mazeInitFuture_;
    QFutureWatcher<std::pair<Maze*, MazeGenerator*>> mazeInitWatcher_;

    static const double roomSize_;
};

#endif // MAZECONTROLLER_H
