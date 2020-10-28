#ifndef NODESCENE_H
#define NODESCENE_H

#include <QVector>

#include "GraphicsScene.h"
#include "GraphicsNode.h"
#include "GraphicsSocket.h"
#include "GraphicsEdge.h"
#include "Node.h"
#include "Edge.h"

class NodeScene
{
public:
    NodeScene();

    void addNode(Node*);
    void removeNode(Node*);

    void addEdge(Edge*);
    void removeEdge(Edge*);

    void connectSockets(Socket*, Socket*);

    QGraphicsScene *graphicsScene;

    void recalculateTargets();
private:
    QVector<Node> nodes;
    QVector<Socket*> sockets;
    QVector<Edge*> edges;


    int sceneWidth = 64000;
    int sceneHeight = 64000;
};

#endif // NODESCENE_H
