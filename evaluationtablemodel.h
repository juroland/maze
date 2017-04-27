#ifndef EVALUATIONTABLEMODEL_H
#define EVALUATIONTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>

#include "model/evaluation.h"

#include <iostream>

class EvaluationTableModel : public QAbstractTableModel
{
public:
    void addEvaluation(const EvaluationReport&);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    std::vector<EvaluationReport> reports_;
};

#endif // EVALUATIONTABLEMODEL_H
