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
        graphicsSocket->setSocketType((int)Type::INTO);
        sockets.insert(input->id, input);
    }

    foreach (auto output, node->getOutputSockets()) {
        auto graphicsSocket = new GraphicsSocket(node->getRenderer());
        graphicsSocket->setPos(output->getSocketLocalPosition());
        graphicsSocket->setIndex(output->index);
        graphicsSocket->setSocketType((int)Type::OUTO);
        sockets.insert(output->id, output);
    }

    nodes.append(node);
}

void NodeScene::removeNode(Node *node)
{

}

void NodeScene::addEdge(Edge *edge)
{
    edge->setRenderer(new GraphicsEdge);
    graphicsScene->addItem(edge->getRenderer());
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
    edges.removeOne(edge);
    edge->detachEdgesFromSockets();
    graphicsScene->removeItem(edge->getRenderer());
}

void NodeScene::recalculateTargets()
{

}

void NodeScene::disconnectEdgeFromSocket(Edge *E, Socket *S)
{
    S->removeEdge(E);
}

void NodeScene::connectEdgeToSocket(Edge *E, Socket *S)
{
    // We always want A to be the output socket and B to be in the input socket
    // When walking the node tree, the recursive function looks for incoming inputs at the current node (A)
    if (S->type == Type::INTO) {
        E->B = S;
    } else {
        E->B = S;
        qSwap(E->A, E->B);
    }

    S->addEdge(E);
    edges.append(E);
}

void NodeScene::connectSockets(Socket *A, Socket *B)
{
    auto edge = new Edge(A, B);
    edge->setRenderer(new GraphicsEdge);
    graphicsScene->addItem(edge->getRenderer());
    edges.append(edge);
}

void NodeScene::walkTree(Node *node, QStringList &code)
{
    if (!node->evaldCode.isNull()) code.prepend(node->evaldCode);
    foreach (auto input, node->getInputSockets()) {
        auto incomingNode = input->getImmediateParent();
        walkTree(incomingNode, code);
    }
}
