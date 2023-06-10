#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "JustJoinItDeserializer.hpp"
#include "OffersDataHelperFunction.hpp"

JustJoinItDeserializer::JustJoinItDeserializer()
{

}

JustJoinItDeserializer::OffersList JustJoinItDeserializer::deserializeOffers(const QByteArray& offersArray)
{
    OffersList offerList;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(offersArray);

    if(jsonDoc.isNull() == false)
    {
        if(jsonDoc.isArray())
        {
            for(const auto& field : jsonDoc.array())
            {
                decltype(offerList)::value_type offer{};
                static_assert(std::is_same<decltype(offer), OffersData>::value, "Incorrect offert data type");

                bool deserializeStatus = deserializeField(field.toObject(), "title", offer.title);
                deserializeStatus = (deserializeField(field.toObject(), "id", offer.id) & deserializeStatus);

                if(deserializeStatus == false)
                {
                    qDebug() << "Deserialize error";
                    break;
                }

                offerList.append(offer);
            }
        }
        else
        {
            qDebug() << "Unsupported json type";
        }
    }
    else
    {
        qDebug() << "Json message is empty";
    }

    return offerList;
}
