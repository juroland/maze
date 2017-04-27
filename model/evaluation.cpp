#include "evaluation.h"
#include "maze.h"

#include "mazesolver.h"
#include "mazegenerator.h"
#include "algorithm.h"

#include <chrono>
#include <valarray>

#include <QVector>

struct EvaluationParameter {
    size_t nrow;
    size_t ncol;
    MazeSolver::ProgramFactory* programFactory;
    MazeGeneratorFactory* generatorFactory;
};

Evaluation evaluate(const EvaluationParameter& param)
{
    auto maze = std::make_unique<Maze>(param.nrow, param.ncol);
    auto solver = std::make_unique<MazeSolver>(maze->getStart(), maze->getEnd());
    solver->start(param.programFactory->makeProgram());

    auto generator = param.generatorFactory->makeGenerator(maze.get());
    while (!generator->isFinished())
        generator->step();

    using clock = std::chrono::steady_clock;
    clock::time_point start = clock::now();

    int nstep = 0;
    while (!solver->isFinished()) {
        ++nstep;
        solver->step();
    }

    clock::time_point end = clock::now();

    Evaluation eval;
    eval.step = nstep;
    auto execTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    eval.execTime = execTime.count()/1000.0;

    return eval;
}

EvaluationReportProducer::EvaluationReportProducer()
{
    QObject::connect(&futureWatcher_, SIGNAL(finished()), this, SLOT(handleFinishedProduce()));
    QObject::connect(&futureWatcher_, SIGNAL(progressRangeChanged(int,int)), this, SIGNAL(progressRangeChanged(int,int)));
    QObject::connect(&futureWatcher_, SIGNAL(progressValueChanged(int)), this, SIGNAL(progressValueChanged(int)));

    programFactories.emplace_back("DFS", std::make_unique<ConcreteProgramFactory<DFSProgram>>());
    programFactories.emplace_back("Custom", std::make_unique<ConcreteProgramFactory<CustomProgram>>());
    generatorFactories.emplace_back("DFS", std::make_unique<ConcreteMazeGeneratorFactory<DFSMazeGenerator>>());
    generatorFactories.emplace_back("Kruskall", std::make_unique<ConcreteMazeGeneratorFactory<KruskallMazeGenerator>>());

}

void EvaluationReportProducer::produce()
{

    QVector<EvaluationParameter> input;
    for (auto& programFactory : programFactories) {
        for (auto& generatorFactory : generatorFactories) {
            std::string title = programFactory.first + " solver on " + generatorFactory.first + " generated Maze";
            evaluationTitles.push_back(title);
            for (size_t i = 0; i < populationSize; ++i) {
                input.push_back(EvaluationParameter{nrow, ncol,
                                                    programFactory.second.get(),
                                                    generatorFactory.second.get()});
            }
        }
    }

    futureWatcher_.setFuture(QtConcurrent::mapped(input, evaluate));
}

std::vector<EvaluationReport> EvaluationReportProducer::reports()
{
    if (isCanceled())
        return {};

    futureWatcher_.waitForFinished();

    std::vector<EvaluationReport> reports;

    size_t resultIndex = 0;
    QList<Evaluation> results = futureWatcher_.future().results();

    std::valarray<double> steps(populationSize);
    double totalExecTime = 0.0;

    for (size_t i = 0; i < evaluationTitles.size(); ++i) {
        for (size_t j = 0; j < populationSize; ++j) {
            Evaluation eval = results.at(static_cast<int>(resultIndex + j));
            steps[j] = eval.step;
            totalExecTime += eval.execTime;
        }
        resultIndex += populationSize;

        EvaluationReport report;
        report.title = evaluationTitles[i];
        report.max = static_cast<int>(steps.max());
        report.min = static_cast<int>(steps.min());
        report.mean = steps.sum()/static_cast<double>(steps.size());
        report.stdDev = std::sqrt(std::pow(steps - report.mean, 2.0).sum()/static_cast<double>(steps.size()));
        report.totalExecTime = totalExecTime;

        reports.emplace_back(report);
    }

    return reports;
}

bool EvaluationReportProducer::isCanceled()
{
    return futureWatcher_.isCanceled();
}

void EvaluationReportProducer::cancel()
{
    futureWatcher_.cancel();
}

void EvaluationReportProducer::handleFinishedProduce()
{
    if (!isCanceled())
            emit resultReady(reports());
    emit finished();
}

