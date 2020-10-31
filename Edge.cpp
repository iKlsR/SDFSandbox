#include "Edge.h"
#include <QDebug>
#include "Socket.h"
#include "Node.h"
#include "GraphicsEdge.h"
#include "GraphicsNode.h"
#include "NodeScene.h"

Edge::Edge()
{

}

Edge::Edge(Socket *a, Socket *b) : A(a), B(b)
{
    id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    A->addEdge(this);
    if (B) B->addEdge(this);
}

void Edge::setRenderer(GraphicsEdge *gEdge) {
    graphicsEdge = gEdge;
//    graphicsEdge->setSource(A->getSocketGlobalPosition());
//    graphicsEdge->setDest(B->getSocketGlobalPosition());
    updateWorldPosition();
}

QJsonObject Edge::serialize()
{
    QVariantMap map;
    map.insert("id", id);
    map.insert("start", A->id);
    map.insert("end", B->id);
    return QJsonObject::fromVariantMap(map);
}

Edge* Edge::deserialize(QJsonObject object, NodeScene *nodeScene)
{
    id = object.value("id").toString();

    QString startingSocketId = object.value("start").toString();
    QString endingSocketId = object.value("end").toString();

    A = nodeScene->sockets.value(startingSocketId);
    B = nodeScene->sockets.value(endingSocketId);

    A->addEdge(this);
    B->addEdge(this);

    return this;
}

GraphicsEdge *Edge::getRenderer() {
    return graphicsEdge;
}

void Edge::updateWorldPosition()
{
    if (A) {
        source = A->getParent()->getRenderer()->pos().toPoint() + A->getSocketLocalPosition();
        graphicsEdge->setSource(source);
    }

    if (B) {
        dest = B->getParent()->getRenderer()->pos().toPoint() + B->getSocketLocalPosition();
        graphicsEdge->setDest(dest);
    } else {
        graphicsEdge->setDest(source);
    }
}
