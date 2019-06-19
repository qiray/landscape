#include <QProcess>
#include <QDebug>
#include <QCoreApplication>
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
    if (!standardOutput.isEmpty()) {
        emit getOutput(standardOutput);
        qDebug() << standardOutput;
    }
    auto standardError = process->readAllStandardError();
    if (!standardError.isEmpty())
        emit error(standardError);
    delete process;
    process = nullptr;
    emit finish();
}

int ExecuteBinary::runBinary(QString path, QStringList args) {
    if (process)
        return 1;
    qDebug() << path << " " << args;
    process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus) { processFinished(exitCode, exitStatus); });
    process->start(QCoreApplication::applicationDirPath() + "/" + path, args);
    return 0;
}
