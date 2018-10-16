#ifndef DRAWMAP_H
#define DRAWMAP_H

#include <QObject>

class DrawMap: public QObject
{
    Q_OBJECT
public:
    explicit DrawMap(QObject *parent = nullptr);
};

#endif // DRAWMAP_H
