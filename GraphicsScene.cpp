#include "GraphicsScene.h"

#include <QPainter>
#include <QBrush>
#include <QColor>

#include <QtDebug>
#include <cmath>

#include "NodeScene.h"

GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    // Setup the background colors here
    backgroundColor = QColor("#393939");
    setBackgroundBrush(backgroundColor);

    primaryGridPen = QPen("#2F2F2F");
    primaryGridPen.setWidth(1);

    secondaryGridPen = QPen("#292929");
    secondaryGridPen.setWidth(2);

    sceneWidth = 8000;
    sceneHeight = 4000;
    gridSpacing = 20, tileSize = 100;

    setSceneRect(-sceneWidth / 2, -sceneHeight / 2, sceneWidth, sceneHeight);

    int canvasLeft = sceneRect().left();
    int canvasRight = sceneRect().right();
    int canvasTop = sceneRect().top();
    int canvasBottom = sceneRect().bottom();

    // Generate horizontal lines
    for (int x = canvasLeft; x < canvasRight; x += gridSpacing) {
        if (x % tileSize != 0) gridLinesPrimary.append(QLine(x, canvasTop, x, canvasBottom));
        else gridLinesSecondary.append(QLine(x, canvasTop, x, canvasBottom));
    }

    // Generate vertical lines
    for (int y = canvasTop; y < canvasBottom; y += gridSpacing) {
        if (y % tileSize != 0) gridLinesPrimary.append(QLine(canvasLeft, y, canvasRight, y));
        else gridLinesSecondary.append(QLine(canvasLeft, y, canvasRight, y));
    }
}

void GraphicsScene::setScene(NodeScene *scene)
{
    this->activeScene = scene;
}

void GraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);

    // Draw the node canvas background
    painter->setPen(primaryGridPen);
    painter->drawLines(gridLinesPrimary);
    painter->setPen(secondaryGridPen);
    painter->drawLines(gridLinesSecondary);
}

//void GraphicsScene::drawForeground(QPainter *painter, const QRectF &rect)
//{
//    QGraphicsScene::drawForeground(painter, rect);
//}
