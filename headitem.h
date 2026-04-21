#ifndef HEADITEM_H
#define HEADITEM_H

#include <QGraphicsObject>

class HeadItem : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit HeadItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;
};

#endif // HEADITEM_H