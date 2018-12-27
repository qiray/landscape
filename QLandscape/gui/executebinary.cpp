#include <QProcess>
#include <QDebug>
#include "executebinary.h"

ExecuteBinary::ExecuteBinary(QObject *parent) : QObject(parent) {

}

int ExecuteBinary::runBinary(QString path, QStringList args) {
    qDebug() << path << " " << args;
    QProcess process(this);
    process.start(path, args);
    process.waitForFinished();
    qDebug() << process.exitCode();
    qDebug() << process.readAllStandardOutput();
    qDebug() << process.readAllStandardError();
    emit finish();
    return 0;
}
