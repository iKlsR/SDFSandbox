#ifndef NODEEDITORGRAPHICSSCENE_H
#define NODEEDITORGRAPHICSSCENE_H

#include <QGraphicsScene>

class NodeScene;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphicsScene(QObject *parent = 0);
    virtual void drawBackground(QPainter*, const QRectF&) override;
//    virtual void drawForeground(QPainter*, const QRectF&) override;

    void setScene(NodeScene*);

private:
    QPen primaryGridPen, secondaryGridPen;
    int sceneWidth, sceneHeight;
    int gridSpacing, tileSize;
    QVector<QLine> gridLinesPrimary, gridLinesSecondary;
    QBrush backgroundColor;

    NodeScene *activeScene;
};

#endif // GRAPHICSSCENE_H
