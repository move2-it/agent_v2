#ifndef MOCKLOGGERWRITTERINTERFACE_HPP
#define MOCKLOGGERWRITTERINTERFACE_HPP

#include <gmock/gmock.h>
#include "LoggerWritterInterface.hpp"

class MockLoggerWritterInterface : public LoggerWritterInterface
{
public:
    MOCK_METHOD(Error_Code_T, create, (), (override));
    MOCK_METHOD(Error_Code_T, print, (const QList<OffertData>& offertData), (override));
};

#endif // MOCKLOGGERWRITTERINTERFACE_HPP
