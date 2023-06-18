#ifndef JUSTJOINITSERIALIZER_HPP
#define JUSTJOINITSERIALIZER_HPP

#include "WebDeserializerInterface.hpp"

class JustJoinItDeserializer : public WebDeserializerInterface
{
public:
    using WebDeserializerInterface::OffersList;

    JustJoinItDeserializer();

    virtual ~JustJoinItDeserializer() = default;

    virtual Error_Code_T deserializeOffers(const QByteArray& offersArray, OffersList& offerList) override;
};

#endif // JUSTJOINITSERIALIZER_HPP
