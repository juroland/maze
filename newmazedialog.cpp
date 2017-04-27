#include "newmazedialog.h"
#include "ui_newmazedialog.h"

#include "model/algorithm.h"

Q_DECLARE_METATYPE(MazeGeneratorFactory*);

NewMazeDialog::NewMazeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewMazeDialog),
    dfsMazeGeneratorFactory_(std::make_unique<ConcreteMazeGeneratorFactory<DFSMazeGenerator>>()),
    kruskallMazeGeneratorFactory_(std::make_unique<ConcreteMazeGeneratorFactory<KruskallMazeGenerator>>()),
    openAllMazeGeneratorFactory_(std::make_unique<ConcreteMazeGeneratorFactory<OpenAllMazeGenerator>>())
{
    ui->setupUi(this);

    ui->generatorComboBox_->addItem("Random DFS", qVariantFromValue(dfsMazeGeneratorFactory_.get()));
    ui->generatorComboBox_->addItem("Random Kruskall", qVariantFromValue(kruskallMazeGeneratorFactory_.get()));
    ui->generatorComboBox_->addItem("Open All", qVariantFromValue(openAllMazeGeneratorFactory_.get()));
}

NewMazeDialog::~NewMazeDialog()
{
    delete ui;
}

size_t NewMazeDialog::nrow() const
{
    return static_cast<size_t>(ui->nrowSpinBox_->value());
}

size_t NewMazeDialog::ncol() const
{
    return static_cast<size_t>(ui->ncolSpinBox_->value());
}

const MazeGeneratorFactory* NewMazeDialog::mazeGeneratorFactory() const
{
    auto index = ui->generatorComboBox_->currentIndex();
    auto selectedFactory = ui->generatorComboBox_->itemData(index).value<MazeGeneratorFactory*>();
    return selectedFactory;
}

int NewMazeDialog::timeStep() const
{
    return ui->timeStepSpinBox_->value();
}
