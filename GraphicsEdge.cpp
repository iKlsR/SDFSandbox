#include "GraphicsEdge.h"

#include <QPainter>
#include <QDebug>

GraphicsEdge::GraphicsEdge() : QGraphicsPathItem()
{
    defaultOutlinePen = QPen(QColor(255, 255, 255, 150));
    defaultOutlinePen.setWidth(4);

    selectedOutlinePen = QPen(Qt::yellow);
    selectedOutlinePen.setWidth(4);

    setFlags(QGraphicsItem::ItemIsSelectable);
}

GraphicsEdge::~GraphicsEdge()
{
    qDebug() << "Edge removed";
}

void GraphicsEdge::updatePath()
{
    painterPath = QPainterPath(src);
    painterPath.lineTo(dst);
    setPath(painterPath);
    setZValue(-1);
}

void GraphicsEdge::setSource(QPoint A)
{
    src = A;
}

void GraphicsEdge::setDest(QPoint B)
{
    dst = B;
}

void GraphicsEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    updatePath();

    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (this->isSelected()) painter->setPen(selectedOutlinePen);
    else painter->setPen(defaultOutlinePen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(painterPath);
}
