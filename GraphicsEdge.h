#ifndef GRAPHICSNOODLE_H
#define GRAPHICSNOODLE_H

#include <QGraphicsLineItem>
#include <QPen>

class GraphicsEdge : public QGraphicsPathItem
{
public:
    GraphicsEdge();
    ~GraphicsEdge();

    enum { Type = UserType + 3 };
    int type() const override {
        return Type;
    }

    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    void updatePath();

    void setSource(QPoint);
    void setDest(QPoint);

private:
    int outlineWidth;
    QPainterPath painterPath;
    QPen defaultOutlinePen;
    QPen selectedOutlinePen;
    QPoint src, dst;
};

#endif // GRAPHICSNOODLE_H
