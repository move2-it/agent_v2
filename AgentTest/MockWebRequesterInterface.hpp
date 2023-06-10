#ifndef MOCKWEBREQUESTERINTERFACE_HPP
#define MOCKWEBREQUESTERINTERFACE_HPP

#include <gmock/gmock.h>
#include "WebRequesterInterface.hpp"

class MockWebRequesterInterface : public WebRequesterInterface
{
public:
    MOCK_METHOD(Error_Code_T, configureRequest, (const QUrl& address), (override));
    MOCK_METHOD(QByteArray, getRequestMessage, (), (override));
    MOCK_METHOD(Error_Code_T, execRequest, (), (override));
};

#endif // MOCKWEBREQUESTERINTERFACE_HPP
