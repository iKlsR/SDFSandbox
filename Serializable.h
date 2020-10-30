#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QJsonObject>
class NodeScene;

class Serializable {
    virtual QJsonObject serialize() = 0;
    virtual Serializable* deserialize(const QJsonObject, NodeScene*) = 0;
};

#endif // SERIALIZABLE_H
