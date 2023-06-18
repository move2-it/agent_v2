#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "JustJoinItDeserializer.hpp"

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
            if(objIn[fieldName].toArray().isEmpty() == false)
            {
                arrayOut = objIn[fieldName].toArray();
                result = true;
            }
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

static Error_Code_T deserializeBasicField(const QJsonObject& jsonObject, OffertData& offertData)
{
    Error_Code_T result = Error_Code_T::ERROR;

    bool deserializeStatus = true;

    deserializeStatus = (deserializeField(jsonObject, "id", offertData.idOffert) & deserializeStatus);
    deserializeStatus = (deserializeField(jsonObject, "title", offertData.titleOffert) & deserializeStatus);
    deserializeStatus = (deserializeField(jsonObject, "company_name", offertData.companyName) & deserializeStatus);
    deserializeStatus = (deserializeField(jsonObject, "city", offertData.city) & deserializeStatus);
    // disabled due to error in api
    deserializeField(jsonObject, "country_code", offertData.country);
    deserializeStatus = (deserializeField(jsonObject, "marker_icon", offertData.areaWork) & deserializeStatus);
    deserializeStatus = (deserializeField(jsonObject, "experience_level", offertData.experienceLevel) & deserializeStatus);
    deserializeStatus = (deserializeField(jsonObject, "workplace_type", offertData.workplace) & deserializeStatus);

    if(deserializeStatus == true)
    {
        result = Error_Code_T::SUCCESS;
    }

    return result;
}

static Error_Code_T deserializeSalary(const QJsonObject& jsonObject, OffertData& offertData)
{
    Error_Code_T result = Error_Code_T::ERROR;

    if(QJsonArray salaryArray; convertJsonObjectToArray(jsonObject, "employment_types", salaryArray) == true)
    {
        bool deserializeStatus = true;

        for(const auto& salaryField : qAsConst(salaryArray))
        {
            bool fieldIsNull;
            deserializeStatus = (checkFieldIsNull(salaryField.toObject(), "salary", fieldIsNull) & deserializeStatus);
            if((deserializeStatus == true) && (fieldIsNull == false))
            {
                decltype(offertData.salary)::value_type salary{};
                static_assert(std::is_same<decltype(salary), Salary>::value, "Incorrect offert salary data type");

                deserializeStatus = (deserializeField(salaryField.toObject(), "type", salary.type) & deserializeStatus);

                QJsonObject salaryObject;
                deserializeStatus = (convertJsonValueToObject(salaryField.toObject()["salary"], salaryObject) & deserializeStatus);
                if(deserializeStatus == true)
                {
                    deserializeStatus = (deserializeField(salaryObject, "currency", salary.currency) & deserializeStatus);
                    deserializeStatus = (deserializeField(salaryObject, "from", salary.min) & deserializeStatus);
                    deserializeStatus = (deserializeField(salaryObject, "to", salary.max) & deserializeStatus);
                }

                if(deserializeStatus == true)
                {
                    offertData.salary.append(std::move(salary));
                }
                else
                {
                    break;
                }
            }
        }

        if(deserializeStatus == true)
        {
            result = Error_Code_T::SUCCESS;
        }
    }

    return result;
}

static Error_Code_T deserializeSkills(const QJsonObject& jsonObject, OffertData& offertData)
{
    Error_Code_T result = Error_Code_T::ERROR;

    if(QJsonArray skillArray; convertJsonObjectToArray(jsonObject, "skills", skillArray))
    {
        bool deserializeStatus = true;

        for(const auto& skillValue : qAsConst(skillArray))
        {
            decltype(offertData.skill)::value_type skill{};
            static_assert(std::is_same<decltype(skill), Skill>::value, "Incorrect offert sill data type");

            deserializeStatus = (deserializeField(skillValue.toObject(), "name", skill.skillName) & deserializeStatus);
            deserializeStatus = (deserializeField(skillValue.toObject(), "level", skill.skillLevel) & deserializeStatus);

            offertData.skill.append(std::move(skill));
        }

        if(deserializeStatus == true)
        {
            result = Error_Code_T::SUCCESS;
        }
    }

    return result;
}

static Error_Code_T deserializeMultilocation(const QJsonObject& jsonObject, OffertData& offertData)
{
    Error_Code_T result = Error_Code_T::ERROR;

    if(QJsonArray multilocationArray; convertJsonObjectToArray(jsonObject, "multilocation", multilocationArray))
    {
        bool deserializeStatus = true;
        for(const auto& multilocationValue : qAsConst(multilocationArray))
        {
            decltype(offertData.multilocation)::value_type multilocation{};
            static_assert(std::is_same<decltype(multilocation), Multilocation>::value, "Incorrect offert multilocation data type");

            deserializeStatus = (deserializeField(multilocationValue.toObject(), "city", multilocation.city) & deserializeStatus);
            deserializeStatus = (deserializeField(multilocationValue.toObject(), "slug", multilocation.slugId) & deserializeStatus);

            offertData.multilocation.append(std::move(multilocation));
        }

        if(deserializeStatus == true)
        {
            result = Error_Code_T::SUCCESS;
        }
    }

    return result;
}

JustJoinItDeserializer::JustJoinItDeserializer()
{

}

Error_Code_T JustJoinItDeserializer::deserializeOffers(const QByteArray& offersArray, OffersList& offerList)
{
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

                OffersList::value_type offer;

                result = deserializeBasicField(field.toObject(), offer);
                if(result == Error_Code_T::ERROR)
                {
                    qDebug() << "Deserialize Basic offert struct Error";
                    break;
                }

                result = deserializeSalary(field.toObject(), offer);
                if(result == Error_Code_T::ERROR)
                {
                    qDebug() << "Deserialize Salary Error";
                    break;
                }

                result = deserializeSkills(field.toObject(), offer);
                if(result == Error_Code_T::ERROR)
                {
                    qDebug() << "Deserialize Skills Error";
                    break;
                }

                result = deserializeMultilocation(field.toObject(), offer);
                if(result == Error_Code_T::ERROR)
                {
                    qDebug() << "Deserialize Multilocation Error";
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

    return result;
}
