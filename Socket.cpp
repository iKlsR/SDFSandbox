#include "Socket.h"
#include "Node.h"
#include "Edge.h"
#include "NodeScene.h"
#include <QDebug>
#include <QJsonObject>

Socket::Socket(Node *p) : parent(p)
{
//    edges.reserve(8);
    id = QUuid::createUuid().toString(QUuid::WithoutBraces);
}

Socket::~Socket()
{
    edges.clear();
}

void Socket::setLocation(Location l)
{
    location = l;
}

QJsonObject Socket::serialize()
{
    QVariantMap map;
    map.insert("id", id);
    map.insert("index", index);
    map.insert("location", QVariant::fromValue((int)location).toJsonValue());
    map.insert("type", QVariant::fromValue((int)type).toJsonValue());
    return QJsonObject::fromVariantMap(map);
}

Socket* Socket::deserialize(QJsonObject object, NodeScene *nodeScene)
{
//    QString startingSocketId = object.value("start").toString();
//    QString endingSocketId = object.value("end").toString();

//    A = nodeScene->sockets.take(startingSocketId);

//    parent = nodeScene->nodes.last();
//    qDebug() << nodeScene;
    edges.reserve(8);

    id = object.value("id").toString();
    index = object.value("index").toInt();
    type = static_cast<Type>(object.value("type").toInt());
    location = static_cast<Location>(object.value("location").toInt());

    return this;
}

void Socket::setLocalSocketPosition(QPoint pos)
{
    socketPosition = pos;
}

bool Socket::hasEdge() {
    return !edges.isEmpty();
}

bool Socket::hasInputEdge()
{
    return edges.first();
}

QPoint Socket::getSocketLocalPosition()
{
    return socketPosition;
}

Node *Socket::getImmediateParent()
{
    if (hasEdge()) {
        return getEdges().first()->getOutputSocket()->getParent();
    }

    return nullptr;
}

QPoint Socket::getSocketGlobalPosition()
{
    return socketPosition + parent->getPosition();
}

