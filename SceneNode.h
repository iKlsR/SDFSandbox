#ifndef SCENENODE_H
#define SCENENODE_H

#include "Node.h"
#include "Edge.h"

class TimeNode : public Node
{
public:
    TimeNode() : Node("Time") {
        type = 313;
        variable = "time_" + variablePostFix;
        setColor(QColor("#f3a683"));
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    }

    QPair<QString, QString> eval() {
        return QPair<QString, QString>(variable, QString("float %1 = %2;")
                .arg(variable, "u_time"));
    }
};

class SinNode : public Node
{
public:
    SinNode() : Node("Sin") {
        type = 313;
        variable = "sin_" + variablePostFix;
        setColor(QColor("#63cdda"));
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    }

    QPair<QString, QString> eval() {
        auto input1 = getInputSockets().first()->getEdges().first()->A->getParent()->eval().first;
        return QPair<QString, QString>(variable, QString("float %1 = sin(%2);").arg(variable, input1));
    }
};

class FloatNode : public Node
{
public:
    FloatNode() : Node("Float") {
        type = 33;
        variable = "float_" + variablePostFix;
        setColor(QColor("#74b9ff"));
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    }

    QPair<QString, QString> eval() {
        QString scale = "1";
        return QPair<QString, QString>(variable, QString("float %1 = %2;")
                .arg(variable, scale));
    }
};

class Vec3Node : public Node
{
public:
    Vec3Node() : Node("Vec3") {
        type = 133;
        variable = "vec3_" + variablePostFix;
        setColor(QColor("#2ecc71"));
        addSocket(Type::INTO, Location::LEFT_TOP);
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::INTO, Location::LEFT_BOTTOM);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    }

    QPair<QString, QString> eval() {
//        auto input1 = getInputSockets().first()
//                ? getInputSockets().first()->getEdges().first()->A->getParent()->eval().first
//                : "0";
//        auto input2 = getInputSockets().at(1)
//                ? getInputSockets().at(1)->getEdges().first()->A->getParent()->eval().first
//                  : "0";
//        auto input3 = getInputSockets().last()
//                ? getInputSockets().last()->getEdges().first()->A->getParent()->eval().first
//                  : "0";

//        QString input1 = "0";
//        QString input2 = "1";
//        QString input3 = "6";

        QVector<QString> inputs = { "0", "1", "6" };
        int index = 0;
        foreach (auto socket, getInputSockets()) {
            if (socket->hasEdge()) {
                inputs[index] = getInputSockets().at(index)->getEdges().first()->A->getParent()->eval().first;
            }
            index++;
        }

        return QPair<QString, QString>(variable, QString("vec3 %1 = vec3(%2, %3, %4);")
                .arg(variable, inputs[0], inputs[1], inputs[2]));
    }
};

class SphereNode : public Node
{
public:
    SphereNode() : Node("Sphere") {
        type = 1;
        variable = "sphere_" + variablePostFix;
        setColor(QColor("#9F7AEA"));
        addSocket(Type::INTO, Location::LEFT_TOP);
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    }

    QPair<QString, QString> eval() {
        auto input1 = getInputSockets().first()->getEdges().first()->A->getParent()->eval().first;
        auto input2 = getInputSockets().last()->getEdges().first()->A->getParent()->eval().first;
        return QPair<QString, QString>(variable, QString("float %1 = sphereSDF(p, %2, %3);")
                .arg(variable, input1, input2));
    }
};

class PlaneNode : public Node
{
public:
    PlaneNode() : Node("Plane") {
        type = 2;
        variable = "plane_" + variablePostFix;
        setColor(QColor("#9F7AEA"));
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    }

    QPair<QString, QString> eval() {;
        return QPair<QString, QString>(variable, QString("float %1 = planeSDF(p);").arg(variable));
    }
};

class MinNode : public Node
{
public:
    MinNode() : Node("Min") {
        type = 3;
        variable = "min_" + variablePostFix;
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::INTO, Location::LEFT_BOTTOM);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        setColor(QColor("#e74c3c"));
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
        setColor(QColor("#596275"));
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
    }

    QPair<QString, QString> eval() {
        return getInputSockets().first()->getEdges().first()->A->getParent()->eval();
    }
};

#endif // SCENENODE_H
