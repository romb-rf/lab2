#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "datamanager.h"
#include "visualwindow.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DataManager dataManager;
    VisualizationWindow visWindow(&dataManager);


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("dataManager", &dataManager);
    engine.rootContext()->setContextProperty("visualizer", &visWindow);
    engine.loadFromModule("lab2", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
