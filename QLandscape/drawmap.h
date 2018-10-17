#ifndef DRAWMAP_H
#define DRAWMAP_H

#include <QObject>
#include <QImage>

class DrawMap: public QObject
{
public:
    explicit DrawMap(QObject *parent = nullptr);
public slots:
    QString generateMap(const QString& rawdata);
    QString generateMapFromFile(const QString& filename);
signals:
    void error(const QString& msg);
private:
    Q_OBJECT
    static QColor getColorFromInt(int c);
    QString imageToBase64(const QImage& image);
};

#endif // DRAWMAP_H
