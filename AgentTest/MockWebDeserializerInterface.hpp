#ifndef MOCKWEBDESERIALIZERINTERFACE_HPP
#define MOCKWEBDESERIALIZERINTERFACE_HPP

#include <gmock/gmock.h>
#include "WebDeserializerInterface.hpp"

class MockWebDeserializerInterface : public WebDeserializerInterface
{
public:
    MOCK_METHOD(Error_Code_T, deserializeOffers, (const QByteArray& offersArray, OffersList& offerList), (override));
};

#endif // MOCKWEBDESERIALIZERINTERFACE_HPP
