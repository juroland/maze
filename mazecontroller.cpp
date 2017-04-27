#include "mazecontroller.h"

#include "mazesolverview.h"
#include "mazeview.h"
#include "mazescene.h"

#include <QtConcurrent>

MazeController::MazeController(MazeView *view)
    : view_(view)
{
    connect(&generationTimer_, &QTimer::timeout, this, &MazeController::onMazeGenerationStep);
    connect(&solveTimer_, &QTimer::timeout, this, &MazeController::onMazeSolveStep);

    connect(&mazeInitWatcher_, &QFutureWatcher<std::pair<Maze*, MazeGenerator*>>::finished,
            this, &MazeController::onMazeInitFinished);
}

void MazeController::newMaze(size_t nrow, size_t ncol, const MazeGeneratorFactory* factory, int timeStep)
{
    generationTimer_.stop();
    solveTimer_.stop();
    generationTimer_.setInterval(timeStep);

    // Init the new maze and its random generator
    auto mazeInit = [nrow, ncol, factory, timeStep]() {
        auto maze = std::make_unique<Maze>(nrow, ncol);

        auto mazeGenerator = factory->makeGenerator(maze.get());
        if (timeStep <= 0) {
            while(!mazeGenerator->isFinished())
                mazeGenerator->step();
        }

        return std::make_pair(maze.release(), mazeGenerator.release());
    };

    mazeInitFuture_ = QtConcurrent::run(mazeInit);
    mazeInitWatcher_.setFuture(mazeInitFuture_);
}

void MazeController::onMazeInitFinished()
{
    auto result = mazeInitFuture_.result();
    maze_.reset(result.first);
    mazeGenerator_.reset(result.second);

    mazeScene_ = std::make_unique<MazeScene>(maze_.get());
    view_->setScene(mazeScene_.get());
    fitMazeInView();

    mazeSolver_ = std::make_unique<MazeSolver>(maze_->getStart(), maze_->getEnd());
    auto solverView = new MazeSolverView(mazeSolver_.get(), mazeScene_.get());
    mazeScene_->addItem(solverView);

    generationTimer_.start();
}

void MazeController::onMazeGenerationStep()
{
    if (mazeGenerator_->isFinished()) {
        generationTimer_.stop();
        emit generationFinished();
        return;
    }
    mazeGenerator_->step();
}

void MazeController::onMazeSolveStep()
{
    mazeSolver_->step();
    if (mazeSolver_->isFinished())
        solveTimer_.stop();
}

void MazeController::solve(const MazeSolver::ProgramFactory* factory, int timeStep)
{
    mazeSolver_->start(factory->makeProgram());

    if (timeStep > 0) {
        solveTimer_.start(timeStep);
    } else {
        while(!mazeSolver_->isFinished())
            mazeSolver_->step();
    }
}

void MazeController::resetSolve()
{
    solveTimer_.stop();
    unmarkRooms(maze_.get());
    mazeSolver_->reset();
}

void MazeController::fitMazeInView()
{
    view_->fitInView(view_->sceneRect(), Qt::KeepAspectRatio);
}
