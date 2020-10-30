#include "MainWindow.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QMenuBar>
#include <QJsonDocument>

#include "GraphicsScene.h"
#include "NodeScene.h"
#include "Node.h"
#include "Socket.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QMenuBar bar;
    bar.addAction("save");
    setMenuBar(&bar);


    auto scene = new NodeScene;

    nodeEditorCanvas = new Canvas(this);
    nodeEditorCanvas->setNodeScene(scene);

    auto A = new Node("Red");
    A->setPosition(-300, -70);
    A->setColor(QColor("#F56565"));
    A->addSocket(Type::OUT, Location::RIGHT_MIDDLE);
    A->addSocket(Type::OUT, Location::RIGHT_BOTTOM);

    auto B = new Node("Indigo");
    B->setColor(QColor("#9F7AEA"));
    B->addSocket(Type::IN, Location::LEFT_MIDDLE);
    B->addSocket(Type::OUT, Location::RIGHT_MIDDLE);

    auto C = new Node("Blue");
    C->setPosition(200, -50);
    C->addSocket(Type::IN, Location::LEFT_MIDDLE);
    C->addSocket(Type::IN, Location::LEFT_BOTTOM);

    auto D = new Node("Green");
    D->setColor(QColor("#48BB78"));
    D->setPosition(50, -150);
    D->addSocket(Type::IN, Location::LEFT_MIDDLE);
    D->addSocket(Type::OUT, Location::RIGHT_MIDDLE);

//    scene->addNode(A);
//    scene->addNode(B);
//    scene->addNode(C);
//    scene->addNode(D);

//    scene->connectSockets(A->getSocketByIndex(Type::OUT, 0), B->getSocketByIndex(Type::IN, 0));
//    scene->connectSockets(A->getSocketByIndex(Type::OUT, 1), D->getSocketByIndex(Type::IN, 0));
//    scene->connectSockets(B->getSocketByIndex(Type::OUT, 0), C->getSocketByIndex(Type::IN, 0));
//    scene.connectSockets(D->getSocketByIndex(Type::OUT, 0), C->getSocketByIndex(Type::IN, 1));
//    scene.connectSockets(A->getSocketByIndex(Type::OUT, 0), C->getSocketByIndex(Type::IN, 0));
//    scene.connectSockets(D->getSocketByIndex(Type::OUT, 0), B->getSocketByIndex(Type::IN, 0));

    setMouseTracking(true);
//    setFocusPolicy(Qt::ClickFocus);
    installEventFilter(this);

    setContentsMargins(0, 0, 0, 0);
    setCentralWidget(nodeEditorCanvas);
}

MainWindow::~MainWindow()
{
    delete nodeEditorCanvas;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape) exit(0);
        if (keyEvent->key() == Qt::Key_S && keyEvent->modifiers() == Qt::ControlModifier) {
            QJsonDocument document;
            document.setObject(nodeEditorCanvas->nodeScene->serialize());
            QFile jsonFile("test.json");
            jsonFile.open(QFile::WriteOnly);
            jsonFile.write(document.toJson());
        }

        if (keyEvent->key() == Qt::Key_N && keyEvent->modifiers() == Qt::ControlModifier) {
            nodeEditorCanvas->clearNodeScene();
        }

        if (keyEvent->key() == Qt::Key_L && keyEvent->modifiers() == Qt::ControlModifier) {
            QFile jsonFile("test.json");
            jsonFile.open(QFile::ReadOnly);
            auto json = QJsonDocument().fromJson(jsonFile.readAll());
            auto scene = new NodeScene;
            scene->deserialize(json.object(), nullptr);
            nodeEditorCanvas->setNodeScene(scene);
        }
    }

    return QObject::eventFilter(watched, event);
}

