#ifndef SOCKET_H
#define SOCKET_H

enum class Type {
    IN = 0,
    OUT
};

enum class Location {
    LEFT_TOP = 0,
    LEFT_MIDDLE,
    LEFT_BOTTOM,
    RIGHT_TOP,
    RIGHT_MIDDLE,
    RIGHT_BOTTOM
};

#include <QPoint>

class Node;
class Edge;
#include <QDebug>
#include "Serializable.h"
class Socket : public Serializable
{
public:
    Socket(Node *parent);
    void setLocation(Location);
    void setLocalSocketPosition(QPoint);
    QPoint getSocketLocalPosition();
    QPoint socketPosition;
    Location location;
    Type type;
    int index;
    Node *parent;
    Node *getParent() {
        return parent;
    }
    void setEdge(Edge*e) {
        edge = e;
    }
    Edge *getEdge() {
        return edge;
    }
    QPoint getSocketGlobalPosition();
    bool hasEdge();
    Edge *edge;
    QString id;

    virtual QJsonObject serialize() override;
    virtual Socket* deserialize(QJsonObject, NodeScene*) override;
};

#endif // SOCKET_H
