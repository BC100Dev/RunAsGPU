#include <QApplication>
#include <QMainWindow>
#include "UI/MainWindow.hpp"

int main(int argc, char** argv, char** envp) {
    // fucking wayland protocol, failing the damn icon
    if (qEnvironmentVariable("XDG_SESSION_TYPE") == "wayland")
        qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));

    QApplication app(argc, argv);
    QMainWindow mainWin;
    Ui_MainWindow ui{};

    ui.setupUi(&mainWin);
    mainWin.show();

    return app.exec();
}