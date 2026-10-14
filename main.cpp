#include <QApplication>
#include "TimerWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TimerWindow window;
    window.show();

    return app.exec();
}