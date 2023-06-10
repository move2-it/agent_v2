#ifndef MOCKWEBDESERIALIZERINTERFACE_HPP
#define MOCKWEBDESERIALIZERINTERFACE_HPP

#include <gmock/gmock.h>
#include "WebDeserializerInterface.hpp"

class MockWebDeserializerInterface : public WebDeserializerInterface
{
public:
    MOCK_METHOD(OffersList, deserializeOffers, (const QByteArray& offersArray), (override));
};

#endif // MOCKWEBDESERIALIZERINTERFACE_HPP
