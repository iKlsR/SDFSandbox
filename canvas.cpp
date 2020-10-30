#include "Canvas.h"

#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QTimeLine>
#include <cmath>

#ifdef QT_DEBUG
    #include <QDebug>
#endif

#include "NodeScene.h"
#include "GraphicsSocket.h"
#include "Edge.h"
#include "Node.h"
#include "GraphicsEdge.h"

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setStyleSheet("border: 0");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

//    setCacheMode(QGraphicsView::CacheBackground);

    installEventFilter(this);
//    setMouseTracking(true);

    // Grimey!
    zoomInFactor = 1.25;
    zoomClamp = false;
    zoom = 10;
    zoomStep = 1;
    zoomRange = {5, 16};

//    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    nodeOp = NodeOperation::NoOp;
}

Canvas::~Canvas()
{
    //    delete graphicsScene;
}

void Canvas::setNodeScene(NodeScene *ns)
{
    nodeScene = ns;
    setScene(nodeScene->graphicsScene);
}

int Canvas::getItemAtClick(QMouseEvent *event)
{
    // Get the entity currently under the cursor
    auto item = itemAt(event->pos());
    // https://doc.qt.io/archives/qt-4.8/qgraphicsitem.html#type
    // https://stackoverflow.com/a/23129179
    // We are looking for sockets whose type is UserType + 2
    if (item) return item->type();
    return -1;
}

void Canvas::edgeDragStart(GraphicsSocket *item)
{
    nodeOp = NodeOperation::EdgeDrag;
//    qDebug() << item->parent->getModel()->getTitle();
    qDebug() << item->getIndex();


    auto startingSocket = item->parent->getModel()->getOutputSockets().at(item->getIndex());
////    auto endSocket = nodeScene->nodes.last()->getInputSockets().first();

    tempDraggingEdge = new Edge(startingSocket, nullptr);
    tempDraggingEdge->setRenderer(new GraphicsEdge(nullptr));
    scene()->addItem(tempDraggingEdge->getRenderer());
}

void Canvas::edgeDragEnd(GraphicsSocket *item)
{
    nodeOp = NodeOperation::NoOp;
    qDebug() << item;
//    auto endSocket = nodeScene->nodes.last()->getInputSockets().first();

    auto endSocket = item->parent->getModel()->getInputSockets().at(item->getIndex());
    nodeScene->connectEdgeToSocket(tempDraggingEdge, endSocket);
}

void Canvas::clearNodeScene()
{
    return;
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (getItemAtClick(event) == QGraphicsItem::UserType + 2) {
            if (nodeOp == NodeOperation::NoOp) {
                edgeDragStart(qgraphicsitem_cast<GraphicsSocket*>(itemAt(event->pos())));
                return;
            }
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    // If we don't complete a node edge Dnd then reset nodeOp
    if (event->button() == Qt::LeftButton) {
        if (getItemAtClick(event) == QGraphicsItem::UserType + 2) {
            if (nodeOp == NodeOperation::EdgeDrag) {
                edgeDragEnd(qgraphicsitem_cast<GraphicsSocket*>(itemAt(event->pos())));
                return;
            }
        } else {
            if (nodeOp == NodeOperation::EdgeDrag) {
                nodeOp = NodeOperation::NoOp;
                delete tempDraggingEdge;
                scene()->removeItem(tempDraggingEdge->getRenderer());
                return;
            }
        }
    }

    if (event->button() == Qt::MiddleButton) {
        setDragMode(QGraphicsView::NoDrag);
    } else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (nodeOp == NodeOperation::EdgeDrag) {
        auto edgeEnd = mapToScene(event->pos()).toPoint();
        tempDraggingEdge->graphicsEdge->setDest(edgeEnd);
        tempDraggingEdge->graphicsEdge->update();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void Canvas::scalingTime(qreal x)
{
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 300.0;
    scale(factor, factor);
}

void Canvas::animFinished() {
    if (_numScheduledScalings > 0) _numScheduledScalings--;
    else _numScheduledScalings++;
    sender()->~QObject();
}

void Canvas::wheelEvent(QWheelEvent *event)
{
    auto angleDelta = event->angleDelta();

#ifdef SMOOTHZOOM
    int numDegrees = angleDelta.y() / 8;

    int numSteps = numDegrees / 15; // see QWheelEvent documentation
    _numScheduledScalings += numSteps;
    if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
        _numScheduledScalings = numSteps;

    QTimeLine *anim = new QTimeLine(200, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(animFinished()));
    anim->start();
#else
    float zoomOutFactor = 1 / zoomInFactor;

    qreal zoomFactor;

    if (angleDelta.y() > 0) {
        zoomFactor = zoomInFactor;
        zoom += zoomStep;
    } else {
        zoomFactor = zoomOutFactor;
        zoom -= zoomStep;
    }

    bool clamped = false;

    if (zoom < zoomRange[0]) { zoom= zoomRange[0]; clamped = true; }
    if (zoom > zoomRange[1]) { zoom= zoomRange[1]; clamped = true; }

    if (!clamped) scale(zoomFactor, zoomFactor);
#endif
}

// Qt's graphics view wants to drag with left click only by default, override this
bool Canvas::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

      if (mouseEvent->button() == Qt::MiddleButton) {
          // Temporarly enable dragging mode
          this->setDragMode(QGraphicsView::ScrollHandDrag);
          // Emit a left mouse click (the default button for the drag mode)
          QMouseEvent* pressEvent = new QMouseEvent(QEvent::GraphicsSceneMousePress,
                                    mouseEvent->pos(), Qt::MouseButton::LeftButton,
                                    Qt::MouseButton::LeftButton, Qt::KeyboardModifier::NoModifier);

          this->mousePressEvent(pressEvent);
      }
      else if (event->type() == QEvent::MouseButtonRelease) {
        // Disable drag mode if dragging is finished
        this->setDragMode(QGraphicsView::DragMode::NoDrag);
      }
    }

  Q_UNUSED(object)
  return false;
}
