#ifndef QFILEWRAPPER_HPP
#define QFILEWRAPPER_HPP

#include <QFile>

class QFileWrapper
{
public:
    QFileWrapper(QString fileName);

    virtual ~QFileWrapper() = default;

    virtual bool open(QIODevice::OpenMode flags);

    virtual void close();

    virtual qint64 write(const char *data);

private:
    QFile file;
};

#endif // QFILEWRAPPER_HPP
