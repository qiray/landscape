#ifndef EXECUTEBINARY_H
#define EXECUTEBINARY_H

#include <QObject>

class ExecuteBinary : public QObject
{
    Q_OBJECT
public:
    explicit ExecuteBinary(QObject *parent = nullptr);

public slots:
    int runBinary(QString path, QStringList args);

signals:
    void error(const QString& msg);

};

#endif // EXECUTEBINARY_H
