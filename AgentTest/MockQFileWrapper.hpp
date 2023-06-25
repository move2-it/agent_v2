#ifndef MOCKQFILEWRAPPER_HPP
#define MOCKQFILEWRAPPER_HPP

#include <gmock/gmock.h>
#include "QFileWrapper.hpp"

class MockQFileWrapper : public QFileWrapper
{
public:
    explicit MockQFileWrapper();

    MOCK_METHOD(bool, open, (QIODevice::OpenMode flags), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(qint64, write, (const char *data), (override));
};

MockQFileWrapper::MockQFileWrapper() :
    QFileWrapper("")
{

};

#endif // MOCKQFILEWRAPPER_HPP
