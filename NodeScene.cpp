#include "NodeScene.h"
#include <QDebug>

NodeScene::NodeScene()
{
    id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    graphicsScene = new GraphicsScene;
}

void NodeScene::addNode(Node *node)
{
    node->setRenderer(new GraphicsNode);
    graphicsScene->addItem(node->getRenderer());

    // A renderer will paint unto its parent so the sockets need the node renderer
    // This is kinda a gotcha so they don't need to be added to the scene separately

    foreach (auto input, node->getInputSockets()) {
        auto graphicsSocket = new GraphicsSocket(node->getRenderer());
        graphicsSocket->setPos(input->getSocketLocalPosition());
        graphicsSocket->setIndex(input->index);
        sockets.insert(input->id, input);
    }

    foreach (auto output, node->getOutputSockets()) {
        auto graphicsSocket = new GraphicsSocket(node->getRenderer());
        graphicsSocket->setPos(output->getSocketLocalPosition());
        graphicsSocket->setIndex(output->index);
        sockets.insert(output->id, output);
    }

    nodes.append(node);
}

void NodeScene::removeNode(Node *node)
{

}

void NodeScene::addEdge(Edge *edge)
{
    edge->setRenderer(new GraphicsEdge(nullptr));
    graphicsScene->addItem(edge->getRenderer());
    edges.append(edge);
}

QJsonObject NodeScene::serialize()
{
    QJsonArray nodeList,edgeList;
    foreach (auto node, nodes) {
        nodeList.append(node->serialize());
    }
    foreach (auto edge, edges) {
        edgeList.append(edge->serialize());
    }

    QVariantMap map;
    map.insert("id", id);
    map.insert("scene_width", sceneWidth);
    map.insert("scene_height", sceneHeight);
    map.insert("nodes", nodeList);
    map.insert("edges", edgeList);
    return QJsonObject::fromVariantMap(map);
}

NodeScene* NodeScene::deserialize(QJsonObject object, NodeScene *scene)
{
    Q_UNUSED(scene);

    // Build nodes
    foreach (auto item, object["nodes"].toArray()) {
        auto node = new Node;
        node->deserialize(item.toObject(), this);
        addNode(node);
    }

    // Build and attach edges
    foreach (auto item, object["edges"].toArray()) {
        auto edge = new Edge;
        edge->deserialize(item.toObject(), this);
        addEdge(edge);
    }

    return this;
}

void NodeScene::removeEdge(Edge *edge)
{

}

void NodeScene::recalculateTargets()
{

}

void NodeScene::connectEdgeToSocket(Edge *E, Socket *S)
{
    E->B = S;
    S->addEdge(E);
    edges.append(E);
}

void NodeScene::connectSockets(Socket *A, Socket *B)
{
    auto edge = new Edge(A, B);
    edge->setRenderer(new GraphicsEdge(nullptr));
    graphicsScene->addItem(edge->getRenderer());
    edges.append(edge);
}
