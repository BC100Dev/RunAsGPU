#include <QApplication>
#include <QMainWindow>
#include "UI/MainWindow.hpp"

int main(int argc, char** argv, char** envp) {
    QApplication app(argc, argv);
    QMainWindow mainWin;
    Ui_MainWindow ui{};

    ui.setupUi(&mainWin);
    mainWin.show();

    return app.exec();
}