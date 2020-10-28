#include "Node.h"
#include <QDebug>
#include "Socket.h"
#include "Edge.h"

Node::Node(const QString &t) : title(t)
{
    pos = QPoint(0, 0);
    setDimensions(QSize(100, 100));
}

void Node::setPosition(int x, int y)
{
    pos.setX(x);
    pos.setY(y);
}

void Node::setColor(const QColor &col)
{
    color = col;
}

QColor Node::getColor()
{
    return color;
}

QPoint Node::getPosition()
{
    return pos;
}

QVector<Socket*> Node::getInputSockets()
{
    return inputs;
}

QVector<Socket*> Node::getOutputSockets()
{
    return outputs;
}

const QString Node::getTitle()
{
    return title;
}

void Node::addSocket(Type type, Location location)
{
    int socketX, socketY;

    Location listL[] = {Location::LEFT_TOP, Location::LEFT_MIDDLE, Location::LEFT_BOTTOM};
    if (std::find(std::begin(listL), std::end(listL), location) != std::end(listL)) {
        socketX = 0;
    }

    Location listR[] = {Location::RIGHT_TOP, Location::RIGHT_MIDDLE, Location::RIGHT_BOTTOM};
    if (std::find(std::begin(listR), std::end(listR), location) != std::end(listR)) {
        socketX = dimensions.width();// - graphicsSocket->boundingRect().width() / 2, 10);
    }

    switch (location) {
        case Location::LEFT_TOP:
        case Location::RIGHT_TOP:
            socketY = 0;
        break;
        case Location::LEFT_MIDDLE:
        case Location::RIGHT_MIDDLE:
            socketY = dimensions.height() / 2;
        break;
        case Location::LEFT_BOTTOM:
        case Location::RIGHT_BOTTOM:
            socketY = dimensions.height();
        break;
        default:
            break;
    }

    auto socket = new Socket(this);
    socket->setLocation(location);
    socket->setLocalSocketPosition(QPoint(socketX, socketY));

    if (type == Type::IN) {
        inputs.append(socket);
    } else {
        outputs.append(socket);
    }
}

void Node::updateConnectedEdges()
{
    for (auto socket : inputs + outputs) {
        if (socket->hasEdge()) socket->getEdge()->updateWorldPosition();
    }
}

