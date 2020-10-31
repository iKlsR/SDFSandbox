#ifndef EDGE_H
#define EDGE_H

#include <QPoint>

#include "Serializable.h"

enum class EdgeType {
    STRAIGHT,
    BEZIER,
    STEP
};

class Socket;
class GraphicsEdge;

class Edge : public Serializable
{
public:
    Edge();
    Edge(Socket*, Socket*);
    QPoint source;
    QPoint dest;

    void setRenderer(GraphicsEdge *gEdge);

    GraphicsEdge *getRenderer();

    void updateWorldPosition();

    GraphicsEdge *graphicsEdge;

    Socket *A;
    Socket *B;

    virtual QJsonObject serialize() override;
    virtual Edge* deserialize(QJsonObject, NodeScene*) override;

    QString id;
};

#endif // EDGE_H
