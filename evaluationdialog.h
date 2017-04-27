#ifndef EVALUATIONDIALOG_H
#define EVALUATIONDIALOG_H

#include <QDialog>

namespace Ui {
class evaluationdialog;
}

#include "evaluationtablemodel.h"
#include "model/evaluation.h"

class EvaluationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EvaluationDialog(QWidget *parent, const std::vector<EvaluationReport>& reports);
    ~EvaluationDialog();

private:
    Ui::evaluationdialog *ui;
    EvaluationTableModel model_;
};

#endif // EVALUATIONDIALOG_H
