#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QByteArray>
#include <QBuffer>

namespace Utils {

QString readStringToNull(QBuffer &buffer) {
    QByteArray result;
    char ch;
    while (buffer.getChar(&ch), ch) {
        result.append(ch);
    }
    return QString(result);
}
}

#endif // UTILS_H
