#ifndef EDGE_H
#define EDGE_H

#include <QPoint>

#include "Serializable.h"

enum class EdgeType {
    STRAIGHT,
    BEZIER,
    STEP
};

#include "Socket.h"
class GraphicsEdge;

class Edge : public Serializable
{
public:
    Edge();
    ~Edge();
    Edge(Socket*, Socket*);
    QPoint source;
    QPoint dest;

    void setRenderer(GraphicsEdge *gEdge);

    GraphicsEdge *getRenderer();

    void updateWorldPosition();

    GraphicsEdge *graphicsEdge;

    void detachEdgesFromSockets() {
        if (A) A->clearEdges();
        if (B) B->clearEdges();
    }

    Socket *getInputSocket() {
        return B;
    }

    Socket *getOutputSocket() {
        return A;
    }

    Socket *A = nullptr;
    Socket *B = nullptr;

    virtual QJsonObject serialize() override;
    virtual Edge* deserialize(QJsonObject, NodeScene*) override;

    QString id;
};

#endif // EDGE_H
