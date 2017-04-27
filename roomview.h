#ifndef CELL_H
#define CELL_H

#include <utility>

#include <QGraphicsItem>

#include "model/maze.h"

class MazeScene;

class RoomView : public QGraphicsItem, public Room::ModificationListener
{
public:
    RoomView(Room* room, MazeScene* mazeScene);

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void update();

    // Room::ModificationListener interface
    void marked() override;
    void unmarked() override;

private:
    Room* room_;
    const MazeScene* mazeScene_;
};


#endif // CELL_H
