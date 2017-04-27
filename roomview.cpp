#include "roomview.h"

#include <QPainter>
#include <utility>
#include <QGraphicsScene>

#include "doorview.h"
#include "roomview.h"
#include "mazescene.h"

RoomView::RoomView(Room *room, MazeScene* mazeScene)
    : room_{room}, mazeScene_{mazeScene}
{
    room->setModificationListener(this);
}

void RoomView::marked()
{
    update();
}

void RoomView::unmarked()
{
    update();
}

QRectF RoomView::boundingRect() const
{
    return mazeScene_->boundingRect(*room_);
}

void RoomView::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (!room_->isMarked())
        return;

    auto margin = boundingRect().width() * 0.4;
    painter->setPen(Qt::white);
    painter->setBrush(Qt::white);
    painter->drawEllipse(boundingRect().marginsRemoved(QMarginsF{margin, margin, margin, margin}));
}

void RoomView::update()
{
    QGraphicsItem::update(boundingRect());
}
