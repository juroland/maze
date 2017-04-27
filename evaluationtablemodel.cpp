#include "evaluationtablemodel.h"

void EvaluationTableModel::addEvaluation(const EvaluationReport& report)
{
    reports_.emplace_back(report);
}

int EvaluationTableModel::rowCount(const QModelIndex&) const
{
    return static_cast<int>(reports_.size());
}

int EvaluationTableModel::columnCount(const QModelIndex&) const
{
    return 5;
}

QVariant EvaluationTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        auto& report = reports_[index.row()];
        switch (index.column()) {
        case 0:
            return report.min;
        case 1:
            return report.max;
        case 2:
            return report.mean;
        case 3:
            return report.stdDev;
        case 4:
            return report.totalExecTime;
        default:
            return QVariant::Invalid;
        }
    }
    return QVariant::Invalid;
}

QVariant EvaluationTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical) {
            return QString(reports_[section].title.c_str());
        } else if(orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return QString("Min");
                case 1:
                    return QString("Max");
                case 2:
                    return QString("Mean");
                case 3:
                    return QString("StdDev");
                case 4:
                    return QString("Total exec. time (s)");
                default:
                    return QVariant::Invalid;
            }
        }
    }

    return QVariant::Invalid;
}
