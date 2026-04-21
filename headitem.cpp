#include "headitem.h"

#include <QPainter>
#include <QPolygonF>

HeadItem::HeadItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
}

QRectF HeadItem::boundingRect() const
{
    return QRectF(0, 0, 30, 30);
}

void HeadItem::paint(QPainter *painter,
                     const QStyleOptionGraphicsItem *,
                     QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(Qt::black);

    painter->drawLine(15, 0, 15, 18);

    QPolygonF triangle;
    triangle << QPointF(15, 28)
             << QPointF(8, 18)
             << QPointF(22, 18);

    painter->drawPolygon(triangle);
}