#ifndef MOCKQNETWORKACCESSMANAGERWRAPPER_HPP
#define MOCKQNETWORKACCESSMANAGERWRAPPER_HPP

#include <gmock/gmock.h>
#include "QNetworkAccessManagerWrapper.hpp"

class MockQNetworkAccessManagerWrapper : public QNetworkAccessManagerWrapper
{
public:
    MOCK_METHOD(QNetworkReply*, get, (const QNetworkRequest& request), (override));
};

#endif // MOCKQNETWORKACCESSMANAGERWRAPPER_HPP
