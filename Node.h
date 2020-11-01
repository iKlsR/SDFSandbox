#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QVector>
#include <QPoint>
#include <QColor>
#include "Socket.h"

#include "Edge.h"
#include "Serializable.h"
#include "GraphicsNode.h"

enum NodeType {
    Time = 0,
    Sin,
    Float,
    Vec3,
    Sphere,
    Plane,
    Min,
    Out
};

class Node : public Serializable
{
public:
    Node(const QString &title = QString());

    void setPosition(int x, int y);
    QPoint getPosition();
    void setColor(const QColor &col);
    QColor getColor();

    // Any node can be eval'd if all the input sockets have edges and the nodes at those edges can be eva'ld
    bool canEval() {
        if (getInputSockets().isEmpty()) return true;
        for (auto socket : getInputSockets()) {
            if (!socket->getEdges().isEmpty()) return false;
            auto parent = socket->getEdges().first()->getOutputSocket()->getParent();
            if (parent) {
                if (parent->canEval()) {
                    return true;
                }
                else return false;
            } else {
                return false;
            }
        }

        return true;
    }

    QString evaldCode;

    QVector<QPair<QString, QString>> code;

    virtual QPair<QString, QString> eval(QStringList &code) {
        return QPair<QString, QString>();
    }

    NodeType type;

    void setDimensions(QSize size) {
        dimensions = size;
    }

    QSize getDimensions() {
        return dimensions;
    }

    QVector<Socket*> getInputSockets();
    QVector<Socket*> getOutputSockets();

    bool allInputsAreValid() {
        for (auto socket : getInputSockets()) {
            if (!socket->hasEdge()) return false;
        }

        return true;
    }

    const QString getTitle();

    Socket *getSocketByIndex(Type type, int index) {
        if (type == Type::INTO) {
            return inputs.at(index);
        } else {
            return outputs.at(index);
        }
    }

    void addSocket(Type, Location);

    void setRenderer(GraphicsNode *gNode) {
        graphicsNode = gNode;

        // Maybe the renderer should read properties from the model?
        graphicsNode->setTitle(this->getTitle());
        graphicsNode->setSize(this->getDimensions());
        graphicsNode->setPos(this->getPosition());
        graphicsNode->setColor(this->getColor());

        graphicsNode->setModel(this);
    }
    GraphicsNode *getRenderer() {
        return graphicsNode;
    }

    void update() {
        updateConnectedEdges();
    }

    void updateConnectedEdges();

    virtual QJsonObject serialize() override;
    virtual Node* deserialize(const QJsonObject, NodeScene *ns) override;

    void addSocket(Socket *socket);
    QPoint getSocketPositionFromLocation(Location location);

    QString id;
protected:
    QString title;
    QString variable;
    QPoint pos;
    QColor color;
    QSize dimensions;
    QVector<Socket*> inputs;
    QVector<Socket*> outputs;

    GraphicsNode *graphicsNode;
    QString variablePostFix;
};

#endif // NODE_H
