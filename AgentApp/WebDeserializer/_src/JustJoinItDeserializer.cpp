#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "JustJoinItDeserializer.hpp"
#include "CommonTypes.hpp"

namespace {

bool checkFieldIsNull(const QJsonObject& objIn, const QString fieldName, bool& resultOut)
{
    bool result = false;

    if(objIn.contains(fieldName))
    {
        if(objIn[fieldName].isNull())
        {
            resultOut = true;
        }
        else
        {
            resultOut = false;
        }
        result = true;
    }

    return result;
}

bool convertJsonValueToObject(const QJsonValueRef& objIn, QJsonObject& objectOut)
{
    bool result = false;

    if(objIn.isObject())
    {
        objectOut = objIn.toObject();
        result = true;
    }

    return result;
}

bool convertJsonObjectToArray(const QJsonObject& objIn, const QString fieldName, QJsonArray& arrayOut)
{
    bool result = false;

    if(objIn.contains(fieldName))
    {
        if(objIn[fieldName].isArray())
        {
            arrayOut = objIn[fieldName].toArray();
            result = true;
        }
    }

    return result;
}

template<typename T>
typename std::enable_if<std::is_convertible<T, QString>::value, bool>::type
deserializeField(const QJsonObject& obj, const QString fieldName, T& field)
{
    bool result = false;

    if(obj.contains(fieldName))
    {
        if(obj[fieldName].isString())
        {
            field = obj[fieldName].toString();
            result = true;
        }
    }

    return result;
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
deserializeField(const QJsonObject& obj, const QString fieldName, T& field)
{
    bool result = false;

    if(obj.contains(fieldName))
    {
        if(obj[fieldName].isDouble())
        {
            field = obj[fieldName].toInt();
            result = true;
        }
    }

    return result;
}

} // end namespace

static Error_Code_T deserializeSalary(const QJsonObject& jsonObject, OffertData& offertData)
{
    Error_Code_T result = Error_Code_T::ERROR;

    QJsonArray salaryArray;
    bool deserializeStatus = convertJsonObjectToArray(jsonObject, "employment_types", salaryArray);

    if(deserializeStatus == true)
    {
        for(const auto& salaryField : qAsConst(salaryArray))
        {
            bool fieldIsNull;
            bool checkResult = checkFieldIsNull(salaryField.toObject(), "salary", fieldIsNull);
            if((checkResult == true) && (fieldIsNull == false))
            {
                decltype(offertData.salary)::value_type salary{};
                static_assert(std::is_same<decltype(salary), Salary>::value, "Incorrect offert salary data type");

                deserializeStatus = (deserializeField(salaryField.toObject(), "type", salary.type) & deserializeStatus);
                if(QJsonObject salaryObject; convertJsonValueToObject(salaryField.toObject()["salary"], salaryObject) == true)
                {
                    deserializeStatus = (deserializeField(salaryObject, "currency", salary.currency) & deserializeStatus);
                    deserializeStatus = (deserializeField(salaryObject, "from", salary.min) & deserializeStatus);
                    deserializeStatus = (deserializeField(salaryObject, "to", salary.max) & deserializeStatus);
                }
                offertData.salary.append(std::move(salary));
            }
        }
    }

    if(deserializeStatus == true)
    {
        result = Error_Code_T::SUCCESS;
    }

    return result;
}

JustJoinItDeserializer::JustJoinItDeserializer()
{

}

JustJoinItDeserializer::OffersList JustJoinItDeserializer::deserializeOffers(const QByteArray& offersArray)
{
    OffersList offerList;
    Error_Code_T result = Error_Code_T::ERROR;

    const QJsonDocument jsonDoc = QJsonDocument::fromJson(offersArray);

    if(jsonDoc.isNull() == false)
    {
        if(jsonDoc.isArray() == true)
        {
            for(const auto& field : static_cast<const QJsonArray>(jsonDoc.array()))
            {
                if(field.isObject() == false)
                {
                    qDebug() << "field in JSON is not object";
                    break;
                }

                decltype(offerList)::value_type offer{};
                static_assert(std::is_same<decltype(offer), OffertData>::value, "Incorrect offert data type");

                result = deserializeSalary(field.toObject(), offer);
                if(result == Error_Code_T::ERROR)
                {
                    qDebug() << "Deserialize Salary Error";
                    break;
                }

                offerList.append(std::move(offer));
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
