#ifndef BYTEREADER_H
#define BYTEREADER_H

#include <QByteArray>
#include <QIODevice>

#include <QBuffer>
#include <QFile>
#include <QtEndian>

template <typename T>
concept Scalar = std::is_scalar_v<T>;

class ByteReader
{
    using u8 = unsigned char;
public:
    ByteReader() {}
    ByteReader(QByteArray *data) {
        this->data = new QBuffer(data);
    }
    ByteReader(const QString &filename) {
        this->data = new QFile(filename);
        this->data->open(QIODevice::ReadOnly);
    }
    ~ByteReader() {
        if (this->data) {
            delete this->data;
            this->data = nullptr;
        }
    }

    template<typename T>
    requires Scalar<T>
    T get() {
        T result;
        QByteArray output = this->data->read(sizeof(T));
        memcpy(&result, output.data(), sizeof(T));
        if (sizeof(T) <= 1) {
            return result;
        }
        return qFromBigEndian(result);
    }

    QByteArray read(size_t size) {
        return this->data->read(size);
    }

private:
    QIODevice *data = nullptr;
};

#endif // BYTEREADER_H
