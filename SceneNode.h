#ifndef SCENENODE_H
#define SCENENODE_H

#include "Node.h"
#include "Edge.h"

class SphereNode : public Node
{
public:
    SphereNode() : Node("Sphere") {
        type = 1;
        variable = "sphere";
        setColor(QColor("#F56565"));
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    }

    QPair<QString, QString> eval() {
        return QPair<QString, QString>(variable, QString("float %1 = sphereSDF(p, %2, %3);")
                .arg(variable, "vec3(0, 1, 6)", "1"));
    }
};

class PlaneNode : public Node
{
public:
    PlaneNode() : Node("Plane") {
        type = 2;
        variable = "plane";
        setColor(QColor("#9F7AEA"));
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    }

    QPair<QString, QString> eval() {;
        return QPair<QString, QString>(variable, "float plane = planeSDF(p);");
    }
};

class MinNode : public Node
{
public:
    MinNode() : Node("Min") {
        type = 3;
        variable = "min1";
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::INTO, Location::LEFT_BOTTOM);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        setColor(QColor("#9F7AEA"));
    }

    QPair<QString, QString> eval() {
        auto input1 = getInputSockets().first()->getEdges().first()->A->getParent()->eval().first;
        auto input2 = getInputSockets().last()->getEdges().first()->A->getParent()->eval().first;
        return QPair<QString, QString>(variable,
                                       QString("float %1 = min(%2, %3);")
                                       .arg(variable, input1, input2));
    }
};


class OutputNode : public Node
{
public:
    OutputNode() : Node("Out") {
        type = 99;
        setColor(QColor("#9F7AEA"));
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
    }

    QPair<QString, QString> eval() {
        return getInputSockets().first()->getEdges().first()->A->getParent()->eval();
    }
};

#endif // SCENENODE_H
