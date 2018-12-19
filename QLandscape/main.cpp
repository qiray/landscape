#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "fileio.h"
#include "drawmap.h"
#include "executebinary.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    qmlRegisterType<FileIO>("FileIO", 1, 0, "FileIO");
    qmlRegisterType<DrawMap>("DrawMap", 1, 0, "DrawMap");
    qmlRegisterType<ExecuteBinary>("ExecuteBinary", 1, 0, "ExecuteBinary");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
