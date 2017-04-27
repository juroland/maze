#include "evaluationdialog.h"
#include "ui_evaluationdialog.h"

#include "model/evaluation.h"
#include "model/algorithm.h"

#include <memory>
#include <QtConcurrent>

EvaluationDialog::EvaluationDialog(QWidget *parent, const std::vector<EvaluationReport> &reports)
    : QDialog(parent),
      ui(new Ui::evaluationdialog)
{
    ui->setupUi(this);

    for (const auto& report : reports)
        this->model_.addEvaluation(report);

    ui->tableView->setModel(&model_);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

EvaluationDialog::~EvaluationDialog()
{
    delete ui;
}
