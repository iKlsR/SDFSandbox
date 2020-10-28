#include "Socket.h"
#include "Node.h"
#include "Edge.h"

#include <QDebug>

Socket::Socket(Node *p) : parent(p)
{
}

void Socket::setLocation(Location l)
{
    location = l;
}

void Socket::setLocalSocketPosition(QPoint pos)
{
    socketPosition = pos;
}

bool Socket::hasEdge() {
    return edge;
}

QPoint Socket::getSocketLocalPosition()
{
    return socketPosition;
}

QPoint Socket::getSocketGlobalPosition()
{
    return socketPosition + parent->getPosition();
}

