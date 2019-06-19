#ifndef EXECUTEBINARY_H
#define EXECUTEBINARY_H

#include <QObject>
#include <QProcess>

class ExecuteBinary : public QObject
{
    Q_OBJECT
public:
    explicit ExecuteBinary(QObject *parent = nullptr);
    ~ExecuteBinary();

public slots:
    int runBinary(QString path, QStringList args);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void error(const QString& msg);
    void getOutput(const QString& msg);
    void finish();
private:
    QProcess* process;
};

#endif // EXECUTEBINARY_H
