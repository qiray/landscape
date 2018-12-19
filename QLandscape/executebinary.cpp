#include <QProcess>
#include "executebinary.h"

ExecuteBinary::ExecuteBinary(QObject *parent) : QObject(parent) {

}

int ExecuteBinary::runBinary(QString path, QStringList args) {
    QProcess P(this);
    P.start(path, args); //TODO: add onfinish call to redraw map
    return 0;
}
