#include "NodeScene.h"
#include <QDebug>

NodeScene::NodeScene()
{
    graphicsScene = new GraphicsScene;
}

void NodeScene::addNode(Node *node)
{
    node->setRenderer(new GraphicsNode);

    // A renderer will paint unto its parent so the sockets need the node renderer
    // This is kinda a gotcha so they don't need to be added to the scene separately

    for (auto input : node->getInputSockets()) {
        auto graphicsSocket = new GraphicsSocket(node->getRenderer());
        graphicsSocket->setPos(input->getSocketLocalPosition());
    }

    for (auto output : node->getOutputSockets()) {
        auto graphicsSocket = new GraphicsSocket(node->getRenderer());
        graphicsSocket->setPos(output->getSocketLocalPosition());
    }

    graphicsScene->addItem(node->getRenderer());
}

void NodeScene::removeNode(Node *node)
{

}

void NodeScene::addEdge(Edge *edge)
{

}

void NodeScene::removeEdge(Edge *edge)
{

}

void NodeScene::recalculateTargets()
{

}

void NodeScene::connectSockets(Socket *A, Socket *B)
{
    auto edge = new Edge(A, B);
    edge->setRenderer(new GraphicsEdge(nullptr));
    graphicsScene->addItem(edge->getRenderer());
    edges.append(edge);
}
