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
    Socket() {}
    Socket(Node *parent);
    ~Socket();
    void setLocation(Location);
    void setLocalSocketPosition(QPoint);
    QPoint getSocketLocalPosition();
    QPoint socketPosition;
    Location location;
    Type type;
    int index;
    Node *parent = nullptr;
    Node *getParent() {
        return parent;
    }
    void addEdge(Edge *e) {
        edges.append(e);
    }

    QVector<Edge*> getEdges() {
        return edges;
    }

    Node *getImmediateParent();

    QPoint getSocketGlobalPosition();
    bool hasEdge();
    bool hasInputEdge();
    void clearEdges() {
        edges.clear();
        edges.squeeze();
    }

    void removeEdge(Edge *e) {
        edges.removeOne(e);
    }
    QString id;

    QVector<Edge*> edges;

    virtual QJsonObject serialize() override;
    virtual Socket* deserialize(QJsonObject, NodeScene*) override;
};

#endif // SOCKET_H
