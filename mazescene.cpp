#include "mazescene.h"

#include "model/maze.h"

#include "roomview.h"
#include "doorview.h"

const double MazeScene::roomSize_ = 50;

MazeScene::MazeScene(Maze *maze)
    : maze_{maze}
{
    QPen pen{QColor{45, 55, 253}, 10, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};

    for (auto& door : maze_->doors()) {
        auto doorView = new DoorView{door.get(), pen, roomSize_};
        addItem(doorView);
    }

    for (auto& room : maze_->rooms()) {
        auto roomView = new RoomView{room.get(), this};
        roomView->setPos(origin(*room));
        addItem(roomView);
    }

    // Borders
    auto nrow = maze_->nrow();
    auto ncol = maze_->ncol();
    QPointF upperLeft = {0, 0};
    QPointF upperRight = {roomSize_*ncol, 0};
    QPointF bottomLeft = {0, roomSize_*nrow};
    QPointF bottomRight = {roomSize_*ncol, roomSize_*nrow};

    setSceneRect(QRectF{upperLeft - QPointF{roomSize_, roomSize_}, bottomRight + QPointF{roomSize_, roomSize_}});

    std::vector<QGraphicsLineItem*> borders(4);
    borders[0] = new QGraphicsLineItem{QLineF{upperLeft, upperRight}};
    borders[1] = new QGraphicsLineItem{QLineF{bottomLeft, bottomRight}};
    borders[2] = new QGraphicsLineItem{QLineF{upperLeft + QPointF{0, roomSize_}, bottomLeft}};
    borders[3] = new QGraphicsLineItem{QLineF{upperRight, bottomRight - QPointF{0, roomSize_}}};

    for (auto border : borders) {
        border->setPen(pen);
        addItem(border);
    }
}

QRectF MazeScene::boundingRect(const Room&) const
{
    return QRectF{QPointF{0, 0}, QPointF{roomSize_, roomSize_}};
}

QPointF MazeScene::origin(const Room& room) const
{
    return QPointF{room.col()*roomSize_, room.row()*roomSize_};
}
