#ifndef MAZESCENE_H
#define MAZESCENE_H

class Maze;
class Room;

#include <QRectF>
#include <QPointF>

#include <QGraphicsScene>
#include <QGraphicsView>

class MazeScene : public QGraphicsScene
{
public:
    MazeScene(Maze* maze);

    QRectF boundingRect(const Room&) const;
    QPointF origin(const Room&) const;

private:
    Maze* maze_;

    const static double roomSize_;
};

#endif // MAZESCENE_H
