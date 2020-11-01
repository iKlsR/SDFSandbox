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

    Node *getSceneNode() {
        for (auto node : nodes) {
            if (node->type == 99) {
                return node;
            }
        }
        return nullptr;
    }

    void walkTree(Node *node, QStringList *code) {
        // For every input connected to this node
        foreach (auto input, node->getInputSockets()) {
            // Get all the edges
            foreach (auto edge, input->getEdges()) {
                // From the edges get the node... repeat
                auto outputSocket = edge->A;
                auto incomingNode = outputSocket->parent;
                code->prepend(incomingNode->eval().second);
                walkTree(incomingNode, code);
            }
        }
    }

    void recalculateTargets();
    void connectEdgeToSocket(Edge *E, Socket *S);

    virtual QJsonObject serialize() override;
    virtual NodeScene* deserialize(QJsonObject, NodeScene*) override;

private:
    int sceneWidth = 64000;
    int sceneHeight = 64000;
    int zoomLevel = 10;
    QString id;
};

#endif // NODESCENE_H
