#include "Socket.h"
#include "Node.h"
#include "Edge.h"

#include <QDebug>
#include <QJsonObject>

Socket::Socket(Node *p) : parent(p)
{
    edge = nullptr;
    id = QUuid::createUuid().toString(QUuid::WithoutBraces);
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
//    parent = nullptr;
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
    return edge != nullptr;
}

QPoint Socket::getSocketLocalPosition()
{
    return socketPosition;
}

QPoint Socket::getSocketGlobalPosition()
{
    return socketPosition + parent->getPosition();
}

