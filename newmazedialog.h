#ifndef NEWMAZEDIALOG_H
#define NEWMAZEDIALOG_H

#include <QDialog>

#include "model/mazegenerator.h"

namespace Ui {
class NewMazeDialog;
}

class NewMazeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewMazeDialog(QWidget *parent = 0);
    ~NewMazeDialog();

    size_t nrow() const;
    size_t ncol() const;

    const MazeGeneratorFactory* mazeGeneratorFactory() const;

    int timeStep() const;

private:
    Ui::NewMazeDialog *ui;

    std::unique_ptr<MazeGeneratorFactory> dfsMazeGeneratorFactory_;
    std::unique_ptr<MazeGeneratorFactory> kruskallMazeGeneratorFactory_;
    std::unique_ptr<MazeGeneratorFactory> openAllMazeGeneratorFactory_;
};

#endif // NEWMAZEDIALOG_H
