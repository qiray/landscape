#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>

class FileIO : public QObject
{
    Q_OBJECT

public:
    explicit FileIO(QObject *parent = nullptr);

public slots:
    QString openFile(QString fileName);
    bool saveFile(QString fileName, const QString &text);

signals:
    void error(const QString& msg);
};

#endif // FILEIO_H
