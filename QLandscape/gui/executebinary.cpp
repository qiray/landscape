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
    int exitCode = process.exitCode();
    if (exitCode)
        qDebug() << process.exitCode();
    auto standardOutput = process.readAllStandardOutput();
    if (!standardOutput.isEmpty())
        qDebug() << standardOutput;
    auto standardError = process.readAllStandardError();
    if (!standardError.isEmpty())
        qDebug() << standardError;
    emit finish();
    return 0;
}
