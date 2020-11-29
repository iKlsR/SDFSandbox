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
#include "SceneNode.h"
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

    connect(nodeEditorCanvas, &Canvas::nodeConnected, this, &MainWindow::refreshRenderer);

    QSizePolicy policy;
    renderer->setSizePolicy(policy);
//    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    windowSplit->addWidget(nodeEditorCanvas);
    windowSplit->addWidget(renderer);
    windowSplit->setSizes(QList<int>({50, 50}));
//    windowSplit->setStretchFactor(0, 2);
//    windowSplit->setStretchFactor(1, 11);

//    auto AA1 = new TimeNode;
//    AA1->setPosition(-800, -120);

//    auto AA2 = new SinNode;
//    AA2->setPosition(-600, -120);

//    auto A1 = new Vec3Node;
//    A1->setPosition(-400, -100);

//    auto A2 = new FloatNode;
//    A2->setPosition(-400, 100);

//    auto A = new SphereNode;
//    A->setPosition(-200, 0);

//    auto B = new PlaneNode;
//    B->setPosition(-200, 200);

//    auto C = new MinNode;
//    C->setPosition(0, 0);

//    auto D = new OutputNode;
//    D->setPosition(200, 0);

//    scene->addNode(AA1);
//    scene->addNode(AA2);
//    scene->addNode(A1);
//    scene->addNode(A2);
//    scene->addNode(A);
//    scene->addNode(B);
//    scene->addNode(C);
//    scene->addNode(D);

//    scene->connectSockets(AA1->getSocketByIndex(Type::OUTO, 0), AA2->getSocketByIndex(Type::INTO, 0));

//    scene->connectSockets(AA2->getSocketByIndex(Type::OUTO, 0), A1->getSocketByIndex(Type::INTO, 1));

//    scene->connectSockets(A1->getSocketByIndex(Type::OUTO, 0), A->getSocketByIndex(Type::INTO, 0));
//    scene->connectSockets(A2->getSocketByIndex(Type::OUTO, 0), A->getSocketByIndex(Type::INTO, 1));

//    scene->connectSockets(A->getSocketByIndex(Type::OUTO, 0), C->getSocketByIndex(Type::INTO, 0));
//    scene->connectSockets(B->getSocketByIndex(Type::OUTO, 0), C->getSocketByIndex(Type::INTO, 1));

//    scene->connectSockets(C->getSocketByIndex(Type::OUTO, 0), D->getSocketByIndex(Type::INTO, 0));

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

        if (keyEvent->key() == Qt::Key_R) {
            refreshRenderer();
        }
    }

    return QObject::eventFilter(watched, event);
}

void MainWindow::refreshRenderer()
{
    auto master = nodeEditorCanvas->nodeScene->getSceneNode();

    if (!master->getInputSockets().first()->getEdges().count()) {
        qDebug() << "Connect to the master node";
        return;
    }

    QStringList code;
    QString lastEvaldFunc = master->eval(code).first;
    QString sceneFunc = QString("float GetDist(vec3 p) { %1 return %2; }").arg(code.join(""), lastEvaldFunc);
//    qDebug() << sceneFunc;
    renderer->recompileFragShader(sceneFunc);
}

