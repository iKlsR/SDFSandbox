#ifndef NODEEDITORCANVAS_H
#define NODEEDITORCANVAS_H

#include <QGraphicsView>

#include "GraphicsScene.h"

class Canvas : public QGraphicsView
{
    Q_OBJECT

public:
    Canvas(QWidget *parent);
    ~Canvas();

//    GraphicsScene *currentScene() {
//        return this->graphicsScene;
//    }

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
};

#endif // CANVAS_H
