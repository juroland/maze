#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "roomview.h"
#include "doorview.h"

#include "newmazedialog.h"
#include "solvemazedialog.h"
#include "evaluationdialog.h"

#include <QGraphicsScene>
#include <QProgressDialog>
#include <QGraphicsLineItem>
#include <QSpinBox>
#include <QtConcurrent>

#include <valarray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mazeController_ = std::make_unique<MazeController>(ui->graphicsView_);
    connect(mazeController_.get(), &MazeController::generationFinished, this, &MainWindow::onGenerationFinished);

    newMazeDialog_ = new NewMazeDialog(this);
    connect(ui->newMazeButton_, &QPushButton::pressed, this, &MainWindow::onNewMazeButtonPressed);

    solveMazeDialog_ = new SolveMazeDialog(this);
    connect(ui->solveButton_, &QPushButton::pressed, this, &MainWindow::onSolveButtonPressed);
    ui->solveButton_->setEnabled(false);

    connect(ui->resetButton_, &QPushButton::pressed, this, &MainWindow::onResetButtonPressed);
    ui->resetButton_->setEnabled(false);

    connect(ui->evaluateButton_, &QPushButton::pressed, this, &MainWindow::onEvaluateButtonPressed);

    ui->graphicsView_->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView_->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView_->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);
    ui->graphicsView_->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
    ui->graphicsView_->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView_->scale(qreal(0.5), qreal(0.5));
    ui->graphicsView_->setBackgroundBrush(Qt::black);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewMazeButtonPressed()
{
    auto code = newMazeDialog_->exec();
    if (code == QDialog::Rejected)
        return;

    mazeController_->newMaze(newMazeDialog_->nrow(),
                             newMazeDialog_->ncol(),
                             newMazeDialog_->mazeGeneratorFactory(),
                             newMazeDialog_->timeStep());

    ui->resetButton_->setEnabled(false);
    ui->solveButton_->setEnabled(false);
}

void MainWindow::onSolveButtonPressed()
{
    auto code = solveMazeDialog_->exec();
    if (code == QDialog::Rejected)
        return;

    auto factory = solveMazeDialog_->programFactory();
    mazeController_->solve(factory, solveMazeDialog_->timeStep());

    ui->solveButton_->setEnabled(false);
    ui->resetButton_->setEnabled(true);
}

void MainWindow::onEvaluateButtonPressed()
{
    QProgressDialog progressDialog{this};
    progressDialog.setWindowTitle("Evaluation");
    progressDialog.setLabelText("Producing the evaluation report..");

    auto producer = new EvaluationReportProducer;

    QObject::connect(producer, SIGNAL(finished()), producer, SLOT(deleteLater()));
    QObject::connect(producer, &EvaluationReportProducer::resultReady,
                     this, &MainWindow::onEvaluationReportReady);

    QObject::connect(producer, SIGNAL(finished()), &progressDialog, SLOT(reset()));
    QObject::connect(&progressDialog, SIGNAL(canceled()), producer, SLOT(cancel()));
    QObject::connect(producer, SIGNAL(progressRangeChanged(int,int)), &progressDialog, SLOT(setRange(int,int)));
    QObject::connect(producer, SIGNAL(progressValueChanged(int)), &progressDialog, SLOT(setValue(int)));

    producer->produce();

    progressDialog.exec();
}

void MainWindow::onEvaluationReportReady(const std::vector<EvaluationReport> &reports)
{
    auto dialog = new EvaluationDialog(this, reports);
    dialog->exec();
}

void MainWindow::onResetButtonPressed()
{
    mazeController_->resetSolve();
    ui->solveButton_->setEnabled(true);
    ui->resetButton_->setEnabled(false);
}

void MainWindow::onGenerationFinished()
{
    ui->solveButton_->setEnabled(true);
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    mazeController_->fitMazeInView();
}
