#ifndef EDGE_H
#define EDGE_H

#include <QPoint>

enum class EdgeType {
    STRAIGHT,
    BEZIER,
    STEP
};

class Socket;
class GraphicsEdge;

class Edge
{
public:
    Edge(Socket*, Socket*);
    QPoint source;
    QPoint dest;

    void setRenderer(GraphicsEdge *gEdge);

    GraphicsEdge *getRenderer();

    void updateWorldPosition();

    GraphicsEdge *graphicsEdge;

    Socket *A;
    Socket *B;
};

#endif // EDGE_H
