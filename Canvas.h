#ifndef NODEEDITORCANVAS_H
#define NODEEDITORCANVAS_H

#include <QGraphicsView>

#include "GraphicsScene.h"

enum class NodeOperation {
    NoOp,
    EdgeDrag,
    EdgeConnected
};

class GraphicsSocket;
class NodeScene;
class Edge;
class Socket;

class Canvas : public QGraphicsView
{
    Q_OBJECT

public:
    Canvas(QWidget *parent);
    ~Canvas();

//    GraphicsScene *currentScene() {
//        return this->graphicsScene;
//    }

    NodeScene *nodeScene;
    void setNodeScene(NodeScene* ns);
    int getItemAtClick(QMouseEvent *event);
    void edgeDragStart(GraphicsSocket *item);
    void edgeDragEnd(GraphicsSocket *item);
    void clearNodeScene();

protected slots:
    void scalingTime(qreal x);
    void animFinished();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

    bool eventFilter(QObject*, QEvent*) override;

private:
//    GraphicsScene *graphicsScene;
    float _numScheduledScalings;

    qreal zoomInFactor;
    bool zoomClamp;
    qreal zoom, zoomStep;
    QVector<int> zoomRange;

    NodeOperation nodeOp;

    Edge *tempDraggingEdge;
};

#endif // CANVAS_H
