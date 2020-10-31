#ifndef SOCKET_H
#define SOCKET_H

#include <QPoint>
#include <QDebug>
#include "Serializable.h"

class Node;
class Edge;

enum class Location {
    LEFT_TOP = 0,
    LEFT_MIDDLE,
    LEFT_BOTTOM,
    RIGHT_TOP,
    RIGHT_MIDDLE,
    RIGHT_BOTTOM
};

enum class Type {
    INTO = 0,
    OUTO
};

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
    void addEdge(Edge *e) {
        edges.append(e);
    }

    QVector<Edge*> getEdges() {
        return edges;
    }

    QPoint getSocketGlobalPosition();
    bool hasEdge();
    QString id;

    QVector<Edge*> edges;

    virtual QJsonObject serialize() override;
    virtual Socket* deserialize(QJsonObject, NodeScene*) override;
};

#endif // SOCKET_H
