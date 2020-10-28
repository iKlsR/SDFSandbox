#include "Edge.h"
#include <QDebug>
#include "Socket.h"
#include "Node.h"
#include "GraphicsEdge.h"
#include "GraphicsNode.h"

Edge::Edge(Socket *a, Socket *b) : A(a), B(b)
{
    A->setEdge(this);
    if (B) B->setEdge(this);
}

void Edge::setRenderer(GraphicsEdge *gEdge) {
    graphicsEdge = gEdge;
//    graphicsEdge->setSource(A->getSocketGlobalPosition());
//    graphicsEdge->setDest(B->getSocketGlobalPosition());
    updateWorldPosition();
}

GraphicsEdge *Edge::getRenderer() {
    return graphicsEdge;
}

void Edge::updateWorldPosition()
{
    auto socketAPos = A->getParent()->getRenderer()->pos().toPoint() + A->getSocketLocalPosition();
    auto socketBPos = B->getParent()->getRenderer()->pos().toPoint() + B->getSocketLocalPosition();

//    qDebug() << "AAAAAAAAAAAAAAAAAAA" << socketAPos;

    graphicsEdge->setSource(socketAPos);
    graphicsEdge->setDest(socketBPos);
}
