#include "GraphicsSocket.h"

GraphicsSocket::GraphicsSocket(QGraphicsItem *parent) : QGraphicsRectItem(parent)
{
    radius = 5;
    outlineWidth = 2;
}

QRectF GraphicsSocket::boundingRect() const
{
   return QRectF(-radius - outlineWidth,
                 -radius - outlineWidth,
                 (radius + outlineWidth) * 2,
                 (radius + outlineWidth) * 2).normalized();
}

void GraphicsSocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPen pen = painter->pen();
    pen.setColor(QColor("#9AE6B4"));
    pen.setWidth(outlineWidth);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor("#68D391")));
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
}

#include <QDebug>
void GraphicsSocket::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug() << "yes";
    QGraphicsRectItem::hoverEnterEvent(event);
}
