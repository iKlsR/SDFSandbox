#ifndef GRAPHICSSOCKET_H
#define GRAPHICSSOCKET_H

#include <QGraphicsItem>
#include <QPainter>

class GraphicsSocket : public QGraphicsRectItem
{
public:
    GraphicsSocket(QGraphicsItem *parent);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

private:
    qreal radius;
    int outlineWidth;
};

#endif // GRAPHICSSOCKET_H
