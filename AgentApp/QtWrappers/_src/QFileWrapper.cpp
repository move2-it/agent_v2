#include "QFileWrapper.hpp"

QFileWrapper::QFileWrapper(QString fileName) :
    file(fileName)
{

}

bool QFileWrapper::open(QIODevice::OpenMode flags)
{
    return file.open(flags);
}

void QFileWrapper::close()
{
    file.close();
}

qint64 QFileWrapper::write(const char *data)
{
    return file.write(data);
}
