#ifndef GRAPHICSSOCKET_H
#define GRAPHICSSOCKET_H

#include <QGraphicsItem>
#include <QPainter>

class GraphicsNode;

class GraphicsSocket : public QGraphicsItem
{
public:
    GraphicsSocket(GraphicsNode *parent);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    enum { Type = UserType + 2 };
    int type() const override {
        return Type;
    }

    int getIndex() { return index; }
    void setIndex(int i) {
        index = i;
    }

    int getSocketType() { return socketType; }
    void setSocketType(int t) {
        socketType = t;
    }

    GraphicsNode *parent;

private:
    qreal radius;
    int outlineWidth;
    int padding;
    int index;
    int socketType;
};

#endif // GRAPHICSSOCKET_H
