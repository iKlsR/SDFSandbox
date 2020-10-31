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
    windowSplit = new QSplitter(Qt::Horizontal, this);
    windowSplit->setHandleWidth(1);

    QMenuBar bar;
    bar.addAction("save");
    setMenuBar(&bar);

    auto scene = new NodeScene;

    nodeEditorCanvas = new Canvas(this);
    nodeEditorCanvas->setNodeScene(scene);

    renderer = new Renderer(this);

    QSizePolicy policy;
    renderer->setSizePolicy(policy);
//    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    windowSplit->addWidget(nodeEditorCanvas);
    windowSplit->addWidget(renderer);
    windowSplit->setSizes(QList<int>({50, 50}));
//    windowSplit->setStretchFactor(0, 2);
//    windowSplit->setStretchFactor(1, 11);

    auto A = new Node("Input");
    A->setPosition(-200, 0);
    A->setColor(QColor("#F56565"));
    A->addSocket(Type::OUTO, Location::RIGHT_MIDDLE);
    scene->addNode(A);

    auto B = new Node("Output");
    A->setPosition(600, 0);
    B->setColor(QColor("#9F7AEA"));
    B->addSocket(Type::INTO, Location::LEFT_MIDDLE);

    scene->addNode(B);

    scene->connectSockets(A->getSocketByIndex(Type::OUTO, 0), B->getSocketByIndex(Type::INTO, 0));

    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
    installEventFilter(this);

    setContentsMargins(0, 0, 0, 0);
    setCentralWidget(windowSplit);
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

        if (keyEvent->key() == Qt::Key_Space) {
            // https://stackoverflow.com/questions/43831474/how-to-equally-distribute-the-width-of-qsplitter
            if (toggleRatio) {
                windowSplit->setSizes(QList<int>({50, 50}));
                toggleRatio = false;
            } else {
                windowSplit->setSizes(QList<int>({70, 30}));
                toggleRatio = true;
            }
        }
    }

    return QObject::eventFilter(watched, event);
}

