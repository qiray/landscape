#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include "fileio.h"

FileIO::FileIO(QObject *parent) :
    QObject(parent)
{
}

QString FileIO::openFile(QString inFileName) {
    inFileName = inFileName.remove(QRegExp("^file://"));
    if (inFileName.isEmpty()){
        emit error(tr("Wrong filename"));
        return QString();
    }

    QFile file(inFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        emit error(tr("Can't read from file") + " '" + inFileName + "'");
        return QString();
    }

    QString line;
    QString fileContent;
    QTextStream t( &file );
    do {
        line = t.readLine();
        fileContent += line + "\n";
    } while (!line.isNull());
    file.close();

    return fileContent;
}

bool FileIO::saveFile(QString fileName, const QString &text) {
    fileName = fileName.remove(QRegExp("^file://"));
    if (fileName.isEmpty()) {
        emit error(tr("Wrong filename"));
        return false;
    }

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        emit error(tr("Can't write to file") + " '" + fileName + "'");
        return false;
    }

    QTextStream out(&file);
    out << text;
    file.close();

    return true;
}
