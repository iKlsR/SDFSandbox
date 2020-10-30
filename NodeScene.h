#ifndef NODESCENE_H
#define NODESCENE_H

#include <QJsonArray>
#include <QVector>

#include "GraphicsScene.h"
#include "GraphicsNode.h"
#include "GraphicsSocket.h"
#include "GraphicsEdge.h"
#include "Node.h"
#include "Edge.h"
#include "Serializable.h"

class NodeScene : public Serializable
{
public:
    NodeScene();

    QVector<Node*> nodes;
    QMap<QString, Socket*> sockets;
    QVector<Edge*> edges;

    void addNode(Node*);
    void removeNode(Node*);

    void addEdge(Edge*);
    void removeEdge(Edge*);

    void connectSockets(Socket*, Socket*);

    QGraphicsScene *graphicsScene;

    void recalculateTargets();
    void connectEdgeToSocket(Edge *E, Socket *S);

    virtual QJsonObject serialize() override;
    virtual NodeScene* deserialize(QJsonObject, NodeScene*) override;

private:
    int sceneWidth = 64000;
    int sceneHeight = 64000;
    QString id;
};

#endif // NODESCENE_H
