#include <QProcess>
#include <QDebug>
#include "executebinary.h"

ExecuteBinary::ExecuteBinary(QObject *parent) : QObject(parent) {
    process = nullptr;
}

ExecuteBinary::~ExecuteBinary() {
    delete process;
}

void ExecuteBinary::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode)
        qDebug() << exitCode << " " << exitStatus;
    auto standardOutput = process->readAllStandardOutput();
    if (!standardOutput.isEmpty())
        qDebug() << standardOutput;
    auto standardError = process->readAllStandardError();
    if (!standardError.isEmpty())
        emit error(standardError);
    emit finish();
}

int ExecuteBinary::runBinary(QString path, QStringList args) {
    qDebug() << path << " " << args;
    process = new QProcess(this); //TODO: delete processes after finish or prevent from their creation
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus) { processFinished(exitCode, exitStatus); });
    process->start("./" + path, args);
    return 0;
}
