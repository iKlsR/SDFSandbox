#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Renderer.h"

#include <QMainWindow>
#include <QSplitter>
#include <QTextEdit>

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
    QTextEdit *textArea;
    QSplitter *windowSplit;
    Renderer *renderer;
    bool toggleRatio = false;
};
#endif // MAINWINDOW_H
