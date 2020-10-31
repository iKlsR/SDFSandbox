#include "Node.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include "Socket.h"
#include "Edge.h"

Node::Node(const QString &t) : title(t)
{
    pos = QPoint(0, 0);
    setDimensions(QSize(100, 100));
    id = QUuid::createUuid().toString(QUuid::WithoutBraces);
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

QJsonObject Node::serialize()
{
    QJsonArray inputList, outputList;
    foreach (auto input, inputs) inputList.append(input->serialize());
    foreach (auto output, outputs) outputList.append(output->serialize());

    QVariantMap map;
    map.insert("id", id);
    map.insert("title", title);
    map.insert("pos_x", getPosition().x());
    map.insert("pos_y", getPosition().y());
    map.insert("inputs", inputList);
    map.insert("outputs", outputList);

    return QJsonObject::fromVariantMap(map);
}

Node* Node::deserialize(const QJsonObject object, NodeScene *nodeScene)
{
    id = object.value("id").toString();
    title = object.value("title").toString();
    setPosition(object.value("pos_x").toInt(), object.value("pos_y").toInt());
//    setColor(QColor("#F56565"));

    foreach (auto input, object.value("inputs").toArray()) {
        auto socket = new Socket(this);
        socket->deserialize(input.toObject(), nodeScene);
        addSocket(socket);
//        addSocket(Type::INTO, static_cast<Location>(input.toObject().value("location").toInt()));
    }

    foreach (auto output, object.value("outputs").toArray()) {
        auto socket = new Socket(this);
        socket->deserialize(output.toObject(), nodeScene);
        addSocket(socket);
//        addSocket(Type::OUTO, static_cast<Location>(output.toObject().value("location").toInt()));
    }

    return this;
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

void Node::addSocket(Socket *socket)
{
    socket->setLocalSocketPosition(getSocketPositionFromLocation(socket->location));

    if (socket->type == Type::INTO) {
        inputs.append(socket);
    } else {
        outputs.append(socket);
    }
}

QPoint Node::getSocketPositionFromLocation(Location location)
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

    return QPoint(socketX, socketY);
}

void Node::addSocket(Type type, Location location)
{
    auto socket = new Socket(this);
    socket->setLocation(location);
    socket->setLocalSocketPosition(getSocketPositionFromLocation(location));

    if (type == Type::INTO) {
        socket->type = Type::INTO;
        socket->index = inputs.length();
        inputs.append(socket);
    } else {
        socket->type = Type::OUTO;
        socket->index = outputs.length();
        outputs.append(socket);
    }
}

void Node::updateConnectedEdges()
{
    foreach (auto socket, inputs + outputs) {
//        if (socket->hasEdge()) socket->getEdge()->updateWorldPosition();
        foreach (auto edge, socket->getEdges()) {
            edge->updateWorldPosition();
        }
    }
}

