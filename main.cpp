#include "MainWindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QScreen>
#include <QDebug>

// Hints that a dedicated GPU should be used whenever possible
// https://stackoverflow.com/a/39047129/991834
#ifdef Q_OS_WIN
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication a(argc, argv);

    QString workingDir = QApplication::applicationDirPath();

    QRect windowSize(QGuiApplication::primaryScreen()->availableGeometry());
    windowSize.adjust(200, 50, -200, -300);

// Let's have a nicer font on Windows and *Nix
#if defined(Q_OS_WIN) || defined(Q_OS_UNIX)
    int id = QFontDatabase::addApplicationFont(workingDir + "/DroidSans.ttf");
    if (id != -1) {
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont monospace(family);
        monospace.setStyleStrategy(QFont::PreferAntialias);
        QApplication::setFont(monospace);
    }
#endif

    MainWindow window;
    window.setWindowTitle("Node Sandbox");
    window.setGeometry(windowSize);
    window.show();

    return a.exec();
}
