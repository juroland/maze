#ifndef GRAPHICAGENT_H
#define GRAPHICAGENT_H

#include <QGraphicsItem>

class Room;
class MazeScene;

#include "model/mazesolver.h"

class MazeSolverView : public QGraphicsItem, public MazeSolver::Observer
{
public:
    MazeSolverView(MazeSolver* mazeSolver, MazeScene* mazeView_);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // MazeSolver::MovementObserver interface
    void moved() override;

private:
    MazeSolver* mazeSolver_;
    MazeScene* mazeView_;
};

#endif // GRAPHICAGENT_H
