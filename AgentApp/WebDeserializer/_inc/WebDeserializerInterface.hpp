#ifndef WEBSERIALIZERINTERFACE_HPP
#define WEBSERIALIZERINTERFACE_HPP

#include <QByteArray>
#include <QList>

#include "OffersData.hpp"
#include "CommonTypes.hpp"

class WebDeserializerInterface
{
public:
    using OffersList = QList<OffertData>;

    WebDeserializerInterface() = default;

    virtual ~WebDeserializerInterface() = default;

    virtual Error_Code_T deserializeOffers(const QByteArray& offersArray, OffersList& offerList) = 0;
};

#endif // WEBSERIALIZERINTERFACE_HPP
