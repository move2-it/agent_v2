#ifndef WEBSERIALIZERINTERFACE_HPP
#define WEBSERIALIZERINTERFACE_HPP

#include <QByteArray>
#include <QList>

#include "OffersData.hpp"

class WebDeserializerInterface
{
public:
    using OffersList = QList<OffertData>;

    WebDeserializerInterface() = default;

    virtual ~WebDeserializerInterface() = default;

    virtual OffersList deserializeOffers(const QByteArray& offersArray) = 0;
};

#endif // WEBSERIALIZERINTERFACE_HPP
