#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QString>
#include <QVector>
#include <QPen>
#include <QFont>

class Node;

class GraphicsNode : public QGraphicsItem
{
public:
    GraphicsNode();

    enum { Type = UserType + 1 };
    int type() const override {
        return Type;
    }

    void setTitle(QString titleText);
    void setColor(QColor);

    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void setModel(Node *m) {
        model = m;
    };
    Node *getModel() {
        return model;
    };

    void setSize(QSize size);
private:
    QString titleText;
    QGraphicsTextItem *textItem;
    QPen selectedOutlinePen;
    QBrush headerBrush, titleBrush, bodyBrush;
    QPen defaultOutlinePen;
    QFont titleFont;
    QColor titleColor;
    QColor headerColor;
    qreal width, height, titleHeight, radius;
    Node *model;
};

#endif // GRAPHICSNODE_H
