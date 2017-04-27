#include "doorview.h"
#include "roomview.h"

#include <QPainter>

DoorView::DoorView(Door *door, QPen pen, double size)
    :  door_(door), pen_(pen)
{
    static QLineF VLine{QPointF{0, 1}, QPointF{0, size-1}};
    static QLineF HLine{QPointF{1, 0}, QPointF{size-1, 0}};

    if (door->direction() == Direction::West || door->direction() == Direction::East)
        line_ = VLine;
    else if (door->direction() == Direction::North || door->direction() == Direction::South)
        line_ = HLine;
    else
        throw std::logic_error{"Invalid orientation"};

    auto rooms = door->rooms();

    // TODO : Move to mazeView
    setPos(std::max(rooms.first->position().col, rooms.second->position().col)*size,
           std::max(rooms.first->position().row, rooms.second->position().row)*size);

    door->setModificationListener(this);
}

QRectF DoorView::boundingRect() const
{
    return QRectF{line_.p1(), line_.p2()}.marginsAdded(QMarginsF{pen_.widthF(), pen_.widthF(), pen_.widthF(), pen_.widthF()});
}

void DoorView::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setPen(pen_);

    if (door_->isClosed())
        painter->drawLine(line_);
}

void DoorView::update()
{
    QGraphicsItem::update(boundingRect());
}

void DoorView::opened()
{
    update();
}

void DoorView::closed()
{
    update();
}
