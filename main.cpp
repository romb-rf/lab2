#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "datamanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    DataManager dataManager;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dataManager", &dataManager);

    engine.loadFromModule("lab2", "Main");
    engine.loadFromModule("lab2", "visual");
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
