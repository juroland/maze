#include "mazesolverview.h"
#include "roomview.h"
#include "mazescene.h"

#include <QPainter>

MazeSolverView::MazeSolverView(MazeSolver* mazeSolver, MazeScene* mazeView)
    : mazeSolver_(mazeSolver),
      mazeView_(mazeView)
{
    mazeSolver->setMovementOberserver(this);
}

QRectF MazeSolverView::boundingRect() const
{
    return mazeView_->boundingRect(*mazeSolver_->currentRoom());
}

void MazeSolverView::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    auto margin = boundingRect().width() * 0.2;
    painter->setPen(Qt::white);
    painter->setBrush(Qt::yellow);
    painter->drawEllipse(boundingRect().marginsRemoved(QMarginsF{margin, margin, margin, margin}));
}


void MazeSolverView::moved()
{
    setPos(mazeView_->origin(*mazeSolver_->currentRoom()));
}
