#include "GraphicsNode.h"

#include <QPainter>
#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>
#include <QDebug>
#include <QStyleOptionGraphicsItem>

#include "Node.h"

GraphicsNode::GraphicsNode()
{
    width = 100;
    height = 100;
    titleHeight = 25;
    radius = 5;

    headerColor = QColor("#4299E1");
    headerBrush = QBrush(headerColor);
    titleBrush = QBrush(Qt::white);
    titleColor = QColor(Qt::white);
    bodyBrush = QBrush(QColor("#2D3748"));

    defaultOutlinePen = QPen(QColor(255, 255, 255, 150));
    defaultOutlinePen.setWidth(4);

    selectedOutlinePen = QPen(Qt::yellow);
    selectedOutlinePen.setWidth(4);

    textItem = new QGraphicsTextItem(this);
    textItem->setPlainText("Untitled");

    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

void GraphicsNode::setSize(QSize size)
{
    width = size.width();
    height = size.height();
}

void GraphicsNode::setTitle(QString title)
{
    QFont font("Droid Sans");
    font.setBold(true);
    font.setPointSize(10);
    textItem->setPlainText(title);
    textItem->setFont(font);
    textItem->setDefaultTextColor(QColor("#FFF"));
    textItem->adjustSize();
    textItem->setPos(QPoint(width / 2 - textItem->boundingRect().width() / 2,
                            (titleHeight / 2 - textItem->boundingRect().height() / 2) + 2));
}

void GraphicsNode::setColor(QColor color)
{
    if (color.isValid()) {
        headerColor = color;
    }
}

QRectF GraphicsNode::boundingRect() const
{
   return QRectF(0, 0,
                 width,
                 height).normalized();
}

void GraphicsNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

//    painter->setClipRect(option->exposedRect);

    headerBrush.setColor(headerColor);

    // Paint the header
    // Rounded corners at the top only https://stackoverflow.com/a/15289912
    // Essentially draw a rounded rect then draw two overlapping rects that cover the gaps
    QPainterPath headerPath;
    headerPath.setFillRule( Qt::WindingFill );
    headerPath.addRoundedRect(QRect(0, 0, width, titleHeight), radius, radius);
    headerPath.addRect(0, titleHeight - radius, radius, radius); // Bottom left corner not rounded
    headerPath.addRect(width - radius, titleHeight - radius, radius, radius); // Bottom right corner not rounded
    painter->setPen(Qt::NoPen);
    painter->setBrush(headerBrush);
    painter->drawPath(headerPath.simplified());

    // Paint the body
    QPainterPath bodyPath;
    bodyPath.addRect(0, titleHeight, width, height - titleHeight);
    painter->setPen(Qt::NoPen);
    painter->setBrush(bodyBrush);
    painter->drawPath(bodyPath.simplified());

    // Paint the border
    QPainterPath borderPath;
    borderPath.addRoundedRect(0, 0, width, height, radius, radius);
    if (this->isSelected()) painter->setPen(selectedOutlinePen);
    else painter->setPen(defaultOutlinePen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(borderPath);
}

void GraphicsNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (model) {
        model->setPosition(scenePos().x(), scenePos().y());
        model->update();
    }

    QGraphicsItem::mouseMoveEvent(event);
}
