#ifndef SOCKET_H
#define SOCKET_H

enum class Type {
    IN,
    OUT
};

enum class Location {
    LEFT_TOP,
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
class Socket
{
public:
    Socket(Node *parent);
    void setLocation(Location);
    void setLocalSocketPosition(QPoint);
    QPoint getSocketLocalPosition();
    QPoint socketPosition;
    Location location;
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
};

#endif // SOCKET_H
