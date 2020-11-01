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

class Node : public Serializable
{
public:
    Node(const QString &title = QString());

    void setPosition(int x, int y);
    QPoint getPosition();
    void setColor(const QColor &col);
    QColor getColor();

    virtual QPair<QString, QString> eval() {
        return QPair<QString, QString>();
    }

    int type;

    void setDimensions(QSize size) {
        dimensions = size;
    }

    QSize getDimensions() {
        return dimensions;
    }

    QVector<Socket*> getInputSockets();
    QVector<Socket*> getOutputSockets();

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

protected:
    QString title;
    QString variable;
    QPoint pos;
    QColor color;
    QSize dimensions;
    QVector<Socket*> inputs;
    QVector<Socket*> outputs;

    GraphicsNode *graphicsNode;
    QString id;
};

#endif // NODE_H
