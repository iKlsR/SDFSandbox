#include "Canvas.h"

#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QTimeLine>
#include <cmath>

#ifdef QT_DEBUG
    #include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#endif

#include "NodeScene.h"
#include "GraphicsSocket.h"
#include "Edge.h"
#include "Node.h"
#include "GraphicsEdge.h"
#include "SceneNode.h"

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
//    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setStyleSheet("border: 0");
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setCacheMode(QGraphicsView::CacheBackground);

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

//    menu = QMenu("Context Menu", this);
    menu.setContextMenuPolicy(Qt::ActionsContextMenu);
    menu.setStyleSheet(
        "QMenu { background-color: #1A1A1A; color: #EEE; padding: 0; margin: 0; }"
        "QMenu::item { background-color: #1A1A1A; padding: 6px 8px; margin: 0; }"
        "QMenu::item:selected { background-color: #3498db; color: #EEE; padding: 6px 8px; margin: 0; }"
        "QMenu::item : disabled { color: #555; }"
    );
}

Canvas::~Canvas()
{
    //    delete graphicsScene;
}

void Canvas::setNodeScene(NodeScene *ns)
{
    nodeScene = ns;
    setScene(nodeScene->graphicsScene);

    auto masterNode = new OutputNode;
    ns->addNode(masterNode);
}

int Canvas::getItemAtClick(QMouseEvent *event)
{
    // Get the entity currently under the cursor
    auto item = itemAt(event->pos());
    // https://doc.qt.io/archives/qt-4.8/qgraphicsitem.html#type
    // https://stackoverflow.com/a/23129179
    // We are looking for sockets whose type is UserType + 2
    // Nodes are UserType + 1 and Edges are UserType + 3
    if (item) return item->type();
    return -1;
}

void Canvas::edgeDragStart(GraphicsSocket *item)
{
    auto startingSocket = item->parent->getModel()
            ->getSocketByIndex(static_cast<Type>(item->getSocketType()), item->getIndex());

    // We are creating if we are dragging from an output socket or an input socket that has no edges
    bool creating = item->getSocketType() == (int) Type::OUTO || (item->getSocketType() == (int) Type::INTO && !startingSocket->hasEdge());

    if (creating) {
        nodeOp = NodeOperation::EdgeDrag;
        qDebug() << "Starting From" << item->getSocketType() << item->getIndex() << startingSocket->getParent()->getTitle() << startingSocket->getParent()->id;
        tempDraggingEdge = new Edge(startingSocket, nullptr);
        nodeScene->addEdge(tempDraggingEdge);
    }
    else {
        nodeOp = NodeOperation::EdgeRemove;
        qDebug() << "Removing" << item->getSocketType() << item->getIndex() << startingSocket->getParent()->getTitle() << startingSocket->getParent()->id;
        tempDraggingEdge = startingSocket->getEdges().first();
    }
}

void Canvas::edgeDragEnd(QMouseEvent *event)
{
    if (getItemAtClick(event) == QGraphicsItem::UserType + 2) {
        if (nodeOp == NodeOperation::EdgeDrag) {
            nodeOp = NodeOperation::NoOp;
            auto graphicsSocket = qgraphicsitem_cast<GraphicsSocket*>(itemAt(event->pos()));
            auto targetSocket = graphicsSocket->parent->getModel()
                    ->getSocketByIndex(static_cast<Type>(graphicsSocket->getSocketType()), graphicsSocket->getIndex());
            nodeScene->connectEdgeToSocket(tempDraggingEdge, targetSocket);
            qDebug() << "Connected to" << graphicsSocket->getSocketType() << graphicsSocket->getIndex() << targetSocket->getParent()->getTitle() << targetSocket->getParent()->id;
            emit nodeConnected();
        }
    } else {
        if (nodeOp == NodeOperation::EdgeDrag) {
            nodeOp = NodeOperation::NoOp;
            nodeScene->removeEdge(tempDraggingEdge);
        } else if (nodeOp == NodeOperation::EdgeRemove) {
            nodeOp = NodeOperation::NoOp;
            nodeScene->removeEdge(tempDraggingEdge);
        }
    }
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
            }

//            return;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        edgeDragEnd(event);
    }

    if (event->button() == Qt::MiddleButton) {
        setDragMode(QGraphicsView::NoDrag);
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (nodeOp == NodeOperation::EdgeDrag || nodeOp == NodeOperation::EdgeRemove) {
        tempDraggingEdge->getRenderer()->setDest(mapToScene(event->pos()).toPoint());
        tempDraggingEdge->getRenderer()->update();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void Canvas::contextMenuEvent(QContextMenuEvent *event)
{
    menu.setContextMenuPolicy(Qt::ActionsContextMenu);
    menu.setStyleSheet(
        "QMenu { background-color: #1A1A1A; color: #EEE; padding: 0; margin: 0; }"
        "QMenu::item { background-color: #1A1A1A; padding: 6px 8px; margin: 0; }"
        "QMenu::item:selected { background-color: #3498db; color: #EEE; padding: 6px 8px; margin: 0; }"
        "QMenu::item : disabled { color: #555; }"
    );

//    QGraphicsDropShadowEffect dropShadowEffect;
//    dropShadowEffect.setBlurRadius(20);
//    dropShadowEffect.setYOffset(4);
//    dropShadowEffect.setXOffset(0);
//    dropShadowEffect.setColor(QColor(0, 0, 0, 32));

//    menu.setGraphicsEffect(&dropShadowEffect);

    QAction floatNode("Float", this);
    QAction vec3Node("Vec3", this);
    QAction timeNode("Time", this);
    QAction sphereNode("Sphere", this);
    QAction planeNode("Plane", this);
    QAction minNode("Min", this);
    QAction sinNode("Sin", this);
    QAction outNode("Out", this);

    QPoint pos = mapToScene(event->pos()).toPoint();

    connect(&floatNode, &QAction::triggered, this, [&]() {
        auto node = new FloatNode;
        node->setPosition(pos.x(), pos.y());
        nodeScene->addNode(node);
    });

    connect(&vec3Node, &QAction::triggered, this, [&]() {
        auto node = new Vec3Node;
        node->setPosition(pos.x(), pos.y());
        nodeScene->addNode(node);
    });

    connect(&timeNode, &QAction::triggered, this, [&]() {
        auto node = new TimeNode;
        node->setPosition(pos.x(), pos.y());
        nodeScene->addNode(node);
    });

    connect(&sphereNode, &QAction::triggered, this, [&]() {
        auto node = new SphereNode;
        node->setPosition(pos.x(), pos.y());
        nodeScene->addNode(node);
    });

    connect(&planeNode, &QAction::triggered, this, [&]() {
        auto node = new PlaneNode;
        node->setPosition(pos.x(), pos.y());
        nodeScene->addNode(node);
    });

    connect(&minNode, &QAction::triggered, this, [&]() {
        auto node = new MinNode;
        node->setPosition(pos.x(), pos.y());
        nodeScene->addNode(node);
    });

    connect(&sinNode, &QAction::triggered, this, [&]() {
        auto node = new SinNode;
        node->setPosition(pos.x(), pos.y());
        nodeScene->addNode(node);
    });

//    connect(&outNode, &QAction::triggered, this, [&]() {
//        auto node = new OutputNode;
//        node->setPosition(pos.x(), pos.y());
//        nodeScene->addNode(node);
//    });

    menu.addAction(&floatNode);
    menu.addAction(&vec3Node);
    menu.addAction(&timeNode);
    menu.addAction(&sphereNode);
    menu.addAction(&planeNode);
    menu.addAction(&minNode);
    menu.addAction(&sinNode);
//    menu.addAction(&outNode);

//    for (auto node : qAsConst(nodeScene->registeredNodes.keys())) {
//        QAction action;
//        action.setText(node->);
//        action->setData(index++);

//        menu.addAction(&action);
//    }

    menu.exec(event->globalPos());
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
    Q_UNUSED(object);

    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if (mouseEvent->button() == Qt::MiddleButton) {
            // Temporarly enable dragging mode
            this->setDragMode(QGraphicsView::ScrollHandDrag);
            // Emit a left mouse click (the default button for the drag mode)
            QMouseEvent* pressEvent = new QMouseEvent(QEvent::GraphicsSceneMousePress,
                                                      mouseEvent->pos(), Qt::MouseButton::LeftButton,
                                                      Qt::MouseButton::LeftButton,
                                                      Qt::KeyboardModifier::NoModifier);

            this->mousePressEvent(pressEvent);
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            // Disable drag mode if dragging is finished
            this->setDragMode(QGraphicsView::DragMode::NoDrag);
        }
    }

    return false;
}
