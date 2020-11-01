QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Canvas.cpp \
    Edge.cpp \
    GraphicsEdge.cpp \
    GraphicsNode.cpp \
    GraphicsScene.cpp \
    GraphicsSocket.cpp \
    MainWindow.cpp \
    Node.cpp \
    NodeScene.cpp \
    Renderer.cpp \
    Socket.cpp \
    main.cpp

HEADERS += \
    Canvas.h \
    Edge.h \
    GraphicsEdge.h \
    GraphicsNode.h \
    GraphicsScene.h \
    GraphicsSocket.h \
    MainWindow.h \
    Node.h \
    NodeScene.h \
    OpCode.h \
    Renderer.h \
    SceneNode.h \
    Serializable.h \
    Socket.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    editor.theme \
    readme.md

RESOURCES += \
    shaders.qrc
