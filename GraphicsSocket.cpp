#include "GraphicsSocket.h"

#include "GraphicsNode.h"

GraphicsSocket::GraphicsSocket(GraphicsNode *p) : QGraphicsItem(p), parent(p)
{
    radius = 8;
    outlineWidth = 2;
    padding = 2; // Make it easier to select
    setZValue(2);
}

QRectF GraphicsSocket::boundingRect() const
{
   return QRectF(-radius - outlineWidth,
                 -radius - outlineWidth,
                 padding + (radius + outlineWidth) * outlineWidth,
                 padding + (radius + outlineWidth) * outlineWidth).normalized();
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

//    painter->drawRect(this->boundingRect());
}
