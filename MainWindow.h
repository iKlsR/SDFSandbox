#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Canvas.h"
class NodeScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject*, QEvent*) override;

private:
    Canvas *nodeEditorCanvas;
    NodeScene *scene;
};
#endif // MAINWINDOW_H
