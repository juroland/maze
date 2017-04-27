#include "solvemazedialog.h"
#include "ui_solvemazedialog.h"

#include "model/algorithm.h"

Q_DECLARE_METATYPE(MazeSolver::ProgramFactory*);

SolveMazeDialog::SolveMazeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolveMazeDialog),
    dfsProgramFactory_(std::make_unique<ConcreteProgramFactory<DFSProgram>>()),
    customProgramFactory_(std::make_unique<ConcreteProgramFactory<CustomProgram>>()),
    randomProgramFactory_(std::make_unique<ConcreteProgramFactory<RandomProgram>>())
{
    ui->setupUi(this);

    ui->solverComboBox_->addItem("DFS", qVariantFromValue(dfsProgramFactory_.get()));
    ui->solverComboBox_->addItem("Custom", qVariantFromValue(customProgramFactory_.get()));
    ui->solverComboBox_->addItem("Random", qVariantFromValue(randomProgramFactory_.get()));
}

SolveMazeDialog::~SolveMazeDialog()
{
    delete ui;
}

int SolveMazeDialog::timeStep() const
{
    return ui->timeStepSpinBox_->value();
}

const MazeSolver::ProgramFactory *SolveMazeDialog::programFactory() const
{
    auto index = ui->solverComboBox_->currentIndex();
    auto selectedFactory = ui->solverComboBox_->itemData(index).value<MazeSolver::ProgramFactory*>();
    return selectedFactory;
}
