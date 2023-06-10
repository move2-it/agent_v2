#ifndef JUSTJOINITSERIALIZER_HPP
#define JUSTJOINITSERIALIZER_HPP

#include "WebDeserializerInterface.hpp"

class JustJoinItDeserializer : public WebDeserializerInterface
{
public:
    using WebDeserializerInterface::OffersList;

    JustJoinItDeserializer();

    virtual ~JustJoinItDeserializer() = default;

    virtual OffersList deserializeOffers(const QByteArray& offersArray) override;
};

#endif // JUSTJOINITSERIALIZER_HPP
