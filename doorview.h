#ifndef SIDE_H
#define SIDE_H

#include <QGraphicsItem>
#include <QRectF>

#include <QPainter>
#include <QPen>

#include "model/maze.h"

#include "roomview.h"

class DoorView : public QGraphicsItem, public Door::ModificationListener {
public:
    DoorView(Door* door, QPen pen, double size);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Door::ModificationListener interface
    void opened() override;
    void closed() override;

private:
    void update();

    Door* door_;
    QPen pen_;
    QLineF line_;
};

#endif // SIDE_H
