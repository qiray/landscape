#include <QDebug>
#include <QBuffer>
#include "drawmap.h"
#include "fileio.h"

DrawMap::DrawMap(QObject *parent) :
    QObject(parent)
{

}

QColor DrawMap::getColorFromInt(int c) {
    if (c < -40)
        return qRgb(0, 0, 32);
    if (c < -10)
        return qRgb(0, 0, 64 + c/2);
    if (c < 1)
        return qRgb(0, 0, 128);
    if (c < 10)
        return qRgb(255, 255, 180 - c*5);
    if (c < 25)
        return qRgb(0, 128 - c*4, 0);
    if (c < 60)
        return qRgb(139 - c*2, 69 - c, 20 - c/3);
    if (c < 100)
        return qRgb(155 + c, 155 + c, 155 + c);
    return qRgb(255, 255, 255);
}

QString DrawMap::imageToBase64(const QImage& image) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    image.save(&buffer, "PNG"); // writes the image in PNG format inside the buffer
    QString iconBase64 = QString::fromLatin1(byteArray.toBase64().data());
    return iconBase64;
}

QString DrawMap::generateMap(const QString& rawdata) {
    if (rawdata == "") {
        emit error(tr("Can't generate map: no data."));
        return QString();
    }
    QStringList mapLines = rawdata.split('\n');
    int len = mapLines.length();
    int mapSize = mapLines[0].toInt();
    QImage img(mapSize, mapSize, QImage::Format_RGB32);
    for (int i = 1; i < len; i++) {
        auto line = mapLines[i].split(' ');
        int lineLength = line.length();
        if (lineLength < mapSize)
            continue;
        for (int j = 0; j < lineLength; j++) {
            bool ok;
            int value = line[j].toInt(&ok);
            if (!ok)
                continue;
            img.setPixelColor(j, i - 1, getColorFromInt(value));
        }
    }
    return imageToBase64(img);
}

QString DrawMap::generateMapFromFile(const QString& filename) {
    FileIO fileIO;
    QString rawdata = fileIO.openFile(filename);
    return generateMap(rawdata);
}
