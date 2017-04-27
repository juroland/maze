#ifndef EVALUATION_H
#define EVALUATION_H

#include <memory>

#include <cstddef>
using std::size_t;

#include "mazesolver.h"
#include "mazegenerator.h"

#include <QObject>

struct EvaluationReport {
    std::string title;
    int max;
    int min;
    double mean;
    double stdDev;
    double totalExecTime;
};

struct Evaluation {
    int step;
    double execTime;
};

#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

class EvaluationReportProducer : public QObject {
    Q_OBJECT

    const size_t nrow = 50;
    const size_t ncol = 100;
    const size_t populationSize = 30;

    QFutureWatcher<Evaluation> futureWatcher_;

    std::vector<std::string> evaluationTitles;

    using NamedProgramFactory = std::pair<std::string, std::unique_ptr<MazeSolver::ProgramFactory>>;
    std::vector<NamedProgramFactory> programFactories;

    using NamedGeneratorFactory = std::pair<std::string, std::unique_ptr<MazeGeneratorFactory>>;
    std::vector<NamedGeneratorFactory> generatorFactories;

public:
    EvaluationReportProducer();

    void produce();
    std::vector<EvaluationReport> reports();

    bool isCanceled();

signals:
    void finished();
    void progressRangeChanged(int,int);
    void progressValueChanged(int);
    void resultReady(const std::vector<EvaluationReport> reports);

public slots:
    void cancel();

private slots:
    void handleFinishedProduce();
};

#endif // EVALUATION_H
