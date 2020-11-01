#ifndef SCENENODE_H
#define SCENENODE_H

#include "Node.h"
#include "Edge.h"

class TimeNode : public Node
{
public:
    TimeNode() : Node("Time") {
        type = NodeType::Time;
        variable = "time_" + variablePostFix;
        setColor(QColor("#f3a683"));
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        evaldCode = "empty";
    }

    QPair<QString, QString> eval(QStringList &code) override {
        evaldCode = QString("float %1 = %2;").arg(variable, "u_time");
        code.append(evaldCode);
        return QPair<QString, QString>(variable, evaldCode);
    }
};

class FloatNode : public Node
{
public:
    FloatNode() : Node("Float") {
        type = NodeType::Float;
        variable = "float_" + variablePostFix;
        setColor(QColor("#74b9ff"));
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        evaldCode = "empty";
    }

    QPair<QString, QString> eval(QStringList &code) override {
        QString argument0 = "1";
        evaldCode = QString("float %1 = %2;").arg(variable, argument0);
        code.append(evaldCode);
        return QPair<QString, QString>(variable, evaldCode);
    }
};

class SinNode : public Node
{
public:
    SinNode() : Node("Sin") {
        type = NodeType::Sin;
        variable = "sin_" + variablePostFix;
        setColor(QColor("#63cdda"));
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        evaldCode = "empty";
    }

    QPair<QString, QString> eval(QStringList &code) override {
        QString argument0 = "0";

        if (getInputSockets().first()->getEdges().count()) {
            argument0 = getInputSockets().first()->getImmediateParent()->eval(code).first;
        }

        evaldCode = QString("float %1 = sin(%2);").arg(variable, argument0);
        code.append(evaldCode);
        return QPair<QString, QString>(variable, evaldCode);
    }
};

class PlaneNode : public Node
{
public:
    PlaneNode() : Node("Plane") {
        type = NodeType::Plane;
        variable = "plane_" + variablePostFix;
        setColor(QColor("#9F7AEA"));
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        evaldCode = "empty";
    }

    QPair<QString, QString> eval(QStringList &code) override {
        evaldCode = QString("float %1 = planeSDF(p);").arg(variable);
        code.append(evaldCode);
        return QPair<QString, QString>(variable, evaldCode);
    }
};

class Vec3Node : public Node
{
public:
    Vec3Node() : Node("Vec3") {
        type = NodeType::Vec3;
        variable = "vec3_" + variablePostFix;
        setColor(QColor("#2ecc71"));
        addSocket(Type::INTO, Location::LEFT_TOP);
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::INTO, Location::LEFT_BOTTOM);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        evaldCode = "empty";
    }

    QPair<QString, QString> eval(QStringList &code) override {
        QString argument0 = "0";
        QString argument1 = "1";
        QString argument2 = "6";

        if (getInputSockets().first()->getEdges().count()) {
            argument0 = getInputSockets().first()->getImmediateParent()->eval(code).first;
        }

        if (getInputSockets().at(1)->getEdges().count()) {
            argument1 = getInputSockets().at(1)->getImmediateParent()->eval(code).first;
        }

        if (getInputSockets().last()->getEdges().count()) {
            argument2 = getInputSockets().last()->getImmediateParent()->eval(code).first;
        }

        evaldCode = QString("vec3 %1 = vec3(%2, %3, %4);").arg(variable, argument0, argument1, argument2);
        code.append(evaldCode);
        return QPair<QString, QString>(variable, evaldCode);
    }
};

class SphereNode : public Node
{
public:
    SphereNode() : Node("Sphere") {
        type = NodeType::Sphere;
        variable = "sphere_" + variablePostFix;
        setColor(QColor("#9F7AEA"));
        addSocket(Type::INTO, Location::LEFT_TOP);
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        evaldCode = "empty";
    }


    QPair<QString, QString> eval(QStringList &code) override {
        QString argument0 = "vec3(0, 1, 6)";
        QString argument1 = "1";

        if (getInputSockets().first()->getEdges().count()) {
            argument0 = getInputSockets().first()->getImmediateParent()->eval(code).first;
        }

        if (getInputSockets().last()->getEdges().count()) {
            argument1 = getInputSockets().last()->getImmediateParent()->eval(code).first;
        }

        evaldCode = QString("float %1 = sphereSDF(p, %2, %3);").arg(variable, argument0, argument1);
        code.append(evaldCode);
        return QPair<QString, QString>(variable, evaldCode);
    }
};

class MinNode : public Node
{
public:
    MinNode() : Node("Min") {
        type = NodeType::Min;
        variable = "min_" + variablePostFix;
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        addSocket(Type::INTO, Location::LEFT_BOTTOM);
        addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
        setColor(QColor("#e74c3c"));
        evaldCode = "empty";
    }

    QPair<QString, QString> eval(QStringList &code) override {
        QString argument0 = "0";
        QString argument1 = "0";

        if (getInputSockets().first()->getEdges().count()) {
            argument0 = getInputSockets().first()->getImmediateParent()->eval(code).first;
        }

        if (getInputSockets().last()->getEdges().count()) {
            argument1 = getInputSockets().last()->getImmediateParent()->eval(code).first;
        }

        evaldCode = QString("float %1 = min(%2, %3);").arg(variable, argument0, argument1);
        code.append(evaldCode);
        return QPair<QString, QString>(variable, evaldCode);
    }
};

class OutputNode : public Node
{
public:
    OutputNode() : Node("Out") {
        type = NodeType::Out;
        setColor(QColor("#596275"));
        addSocket(Type::INTO, Location::LEFT_MIDDLE);
        evaldCode = "empty";
    }

    QPair<QString, QString> eval(QStringList &code) override {
        Q_UNUSED(code);
        return getInputSockets().first()->getImmediateParent()->eval(code);
    }
};

#endif // SCENENODE_H
