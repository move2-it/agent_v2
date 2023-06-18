#include <gtest/gtest.h>
#include <regex>
#include <vector>
#include <algorithm>
#include <type_traits>

#include "JustJoinItDeserializer.hpp"

class JsonJustJoinIt
{
public:
    JsonJustJoinIt() {}

    void createBasis();
    void addEmploymentTypesField(int quantity);
    void addEmploymentSalaryField();
    void addSkillsField(int quantity);
    void addMultilocationField(int quantity);
    void makeValid();

    void removeField(QString field);

    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value && std::is_same<T, bool>::value>::type
    changeField(QString field, T value);
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value>::type
    changeField(QString field, T value);
    void changeField(QString field, QString value, bool checkValue = true);

    QByteArray& getRefJson();

private:
    QByteArray json{};
};

class JustJoinItDeserializerTest : public ::testing::Test
{
public:
    JustJoinItDeserializerTest()
    {
        json.createBasis();
    }

protected:
    JustJoinItDeserializer deserializer;
    QList<OffertData> offertData{};
    JsonJustJoinIt json;
};

void JsonJustJoinIt::createBasis()
{
    json = R"([
      {
        "title": "json.title",
        "street": "json.street",
        "city": "json.city",
        "country_code": "json.country_code",
        "address_text": "json.address_text",
        "marker_icon": "json.marker_icon",
        "workplace_type": "json.workplace_type",
        "company_name": "json.company_name",
        "company_url": "json.company_url",
        "company_size": "json.company_size",
        "experience_level": "json.experience_level",
        "latitude": "json.latitude",
        "longitude": "json.longitude",
        "published_at": "json.published_at",
        "remote_interview": "json.remote_interview",
        "open_to_hire_ukrainians": "json.open_to_hire_ukrainians",
        "id": "json.id",
        "display_offer": "json.display_offer",
        "employment_types": "json.employment_types",
        "company_logo_url": "json.company_logo_url",
        "skills": "json.skills",
        "remote": "json.remote",
        "multilocation": "json.multilocation",
        "way_of_apply": "json.way_of_apply"
      }
    ])";
}

void JsonJustJoinIt::addEmploymentTypesField(int quantity)
{
    const QString field = R"(          {
            "type": "json.employment_types.type",
            "salary": "json.employment_types.salary"
          })";

    if(json.indexOf("json.employment_types") != -1)
    {
        std::vector<QString> fields(quantity, field);

        QString fieldToAdd = std::accumulate(fields.begin(), fields.end(), QString{},
            [](const QString& accumulator, QString& current) {
                return accumulator.isEmpty() ? current : accumulator + ",\n" + current;
            });
        fieldToAdd.push_front("[\n");
        fieldToAdd.push_back(']');

        changeField("json.employment_types", fieldToAdd, false);
    }
}

void JsonJustJoinIt::addEmploymentSalaryField()
{
    const QString field = R"({
              "from": "json.employment_types.salary.from",
              "to": "json.employment_types.salary.to",
              "currency": "json.employment_types.salary.currency"
            })";

    if(json.toStdString().find("json.employment_types.salary") != std::string::npos)
    {
        changeField("json.employment_types.salary", field, false);
    }
}

void JsonJustJoinIt::addSkillsField(int quantity)
{
    const QString field = R"(          {
            "name": "json.skills.name",
            "level": "json.skills.level"
          })";

    if(json.indexOf("json.skills") != -1)
    {
        std::vector<QString> fields(quantity, field);

        QString fieldToAdd = std::accumulate(fields.begin(), fields.end(), QString{},
            [](const QString& accumulator, QString& current) {
                return accumulator.isEmpty() ? current : accumulator + ",\n" + current;
            });
        fieldToAdd.push_front("[\n");
        fieldToAdd.push_back(']');

        changeField("json.skills", fieldToAdd, false);
    }
}

void JsonJustJoinIt::addMultilocationField(int quantity)
{
    const QString field = R"(          {
            "city": "json.multilocation.city",
            "street": "json.multilocation.street",
            "slug": "json.multilocation.slug"
          })";

    if(json.indexOf("json.multilocation") != -1)
    {
        std::vector<QString> fields(quantity, field);

        QString fieldToAdd = std::accumulate(fields.begin(), fields.end(), QString{},
            [](const QString& accumulator, QString& current) {
                return accumulator.isEmpty() ? current : accumulator + ",\n" + current;
            });
        fieldToAdd.push_front("[\n");
        fieldToAdd.push_back(']');

        changeField("json.multilocation", fieldToAdd, false);
    }
}

void JsonJustJoinIt::makeValid()
{
    if(json.indexOf("json.remote_interview") != -1)
    {
        changeField("json.remote_interview", false);
    }
    if(json.indexOf("json.open_to_hire_ukrainians") != -1)
    {
        changeField("json.open_to_hire_ukrainians", false);
    }
    if(json.indexOf("json.display_offer") != -1)
    {
        changeField("json.display_offer", false);
    }
    if(json.indexOf("json.remote") != -1)
    {
        changeField("json.remote", false);
    }
    if(json.indexOf("json.employment_types") != -1)
    {
        addEmploymentTypesField(1);
    }
    if(json.indexOf("json.employment_types.salary") != -1)
    {
        changeField("json.employment_types.salary", "null", false);
    }
    if(json.indexOf("json.employment_types.salary.from") != -1)
    {
        changeField("json.employment_types.salary.from", 0);
    }
    if(json.indexOf("json.employment_types.salary.to") != -1)
    {
        changeField("json.employment_types.salary.to", 0);
    }
    if(json.indexOf("json.skills") != -1)
    {
        addSkillsField(1);
    }
    if(json.indexOf("json.skills.level") != -1)
    {
        changeField("json.skills.level", 0);
    }
    if(json.indexOf("json.multilocation") != -1)
    {
        addMultilocationField(1);
    }
}

void JsonJustJoinIt::removeField(QString field)
{
    if(auto pos = json.indexOf(field.toUtf8()); pos != -1)
    {
        auto lineStart = json.lastIndexOf('\n', pos);
        auto lineEnd = json.indexOf('\n', pos);
        json.remove(lineStart, lineEnd - lineStart);

        lineEnd = ++lineStart;
        while(json[lineEnd] != '\n')
        {
            lineEnd++;
        }

        if(json.mid(lineStart, lineEnd - lineStart + 1).indexOf("\"json.") == -1)
        {
            json.remove(lineStart-2, 1);
        }
    }
}

static void checkField(QString& field)
{
    if(*field.begin() != '\"')
    {
        field.prepend('\"');
    }
    if(*(field.end()-1) != '\"')
    {
        field.push_back('\"');
    }
}

void JsonJustJoinIt::changeField(QString field, QString value, bool checkValue)
{
    checkField(field);
    if(checkValue == true)
    {
        checkField(value);
    }
    std::regex regex(field.toStdString());
    json = std::regex_replace(json.toStdString(), regex, value.toStdString()).c_str();
}

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value && std::is_same<T, bool>::value>::type
JsonJustJoinIt::changeField(QString field, T value)
{
    checkField(field);
    std::string boolValue = value ? "true" : "false";
    std::regex regex(field.toStdString());
    json = std::regex_replace(json.toStdString(), regex, boolValue).c_str();
}

template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value>::type
JsonJustJoinIt::changeField(QString field, T value)
{
    checkField(field);
    std::regex regex(field.toStdString());
    json = std::regex_replace(json.toStdString(), regex, std::to_string(value)).c_str();
}

QByteArray& JsonJustJoinIt::getRefJson()
{
    return json;
}

TEST_F(JustJoinItDeserializerTest, deserializeError_jsonIsEmpty)
{
    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(QByteArray{}, offertData));
    EXPECT_EQ(true, offertData.isEmpty());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_jsonIsNull)
{
    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(QByteArray{"null"}, offertData));
    EXPECT_EQ(true, offertData.isEmpty());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_jsonIsEmptyArray)
{
    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(QByteArray{R"({[]})"}, offertData));
    EXPECT_EQ(true, offertData.isEmpty());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_jsonIsNullArray)
{
    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(QByteArray{R"({[null]})"}, offertData));
    EXPECT_EQ(true, offertData.isEmpty());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_jsonIsNoArray)
{
    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(QByteArray{R"({"json": "test"})"}, offertData));
    EXPECT_EQ(true, offertData.isEmpty());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_jsonHaveEmptyField)
{
    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(QByteArray{R"({"json": })"}, offertData));
    EXPECT_EQ(true, offertData.isEmpty());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_jsonArrayIsNoObject)
{
    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(QByteArray{R"(["Hello"])"}, offertData));
    EXPECT_EQ(true, offertData.isEmpty());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_idField)
{
    const QString idField{"idField"};
    json.changeField("json.id", idField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(idField, offertData[0].idOffert);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_idFieldIsNull)
{
    json.changeField("json.id", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_idFieldIsNumber)
{
    json.changeField("json.id", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_idFieldNoExist)
{
    json.removeField("json.id");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_titleField)
{
    const QString titleField{"titleField"};
    json.changeField("json.title", titleField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(titleField, offertData[0].titleOffert);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_titleFieldIsNull)
{
    json.changeField("json.title", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_titleFieldIsNumber)
{
    json.changeField("json.title", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_titleFieldNoExist)
{
    json.removeField("json.title");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_companyNameField)
{
    const QString companyNameField{"companyNameField"};
    json.changeField("json.company_name", companyNameField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(companyNameField, offertData[0].companyName);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_companyNameFieldIsNull)
{
    json.changeField("json.company_name", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_companyNameFieldIsNumber)
{
    json.changeField("json.company_name", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_companyNameFieldNoExist)
{
    json.removeField("json.company_name");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_cityField)
{
    const QString cityField{"cityField"};
    json.changeField("json.city", cityField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(cityField, offertData[0].city);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_cityFieldIsNull)
{
    json.changeField("json.city", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_cityFieldIsNumber)
{
    json.changeField("json.city", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_cityFieldNoExist)
{
    json.removeField("json.city");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_countryCodeField)
{
    const QString countryCodeField{"countryCodeField"};
    json.changeField("json.country_code", countryCodeField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(countryCodeField, offertData[0].country);
}

// disabled due to error in api
//TEST_F(JustJoinItDeserializerTest, deserializeError_countryCodeFieldIsNull)
//{
//    json.changeField("json.country_code", "null", false);
//    json.makeValid();

//    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
//    EXPECT_EQ(0, offertData.length());
//}

// disabled due to error in api
//TEST_F(JustJoinItDeserializerTest, deserializeError_countryCodeFieldIsNumber)
//{
//    json.changeField("json.country_code", 0);
//    json.makeValid();

//    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
//    EXPECT_EQ(0, offertData.length());
//}

// disabled due to error in api
//TEST_F(JustJoinItDeserializerTest, deserializeError_countryCodeFieldNoExist)
//{
//    json.removeField("json.country_code");
//    json.makeValid();

//    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
//    EXPECT_EQ(0, offertData.length());
//}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_markerIconField)
{
    const QString markerIconField{"markerIconField"};
    json.changeField("json.marker_icon", markerIconField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(markerIconField, offertData[0].areaWork);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_markerIconFieldIsNull)
{
    json.changeField("json.marker_icon", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_markerIconFieldIsNumber)
{
    json.changeField("json.marker_icon", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_markerIconFieldNoExist)
{
    json.removeField("json.marker_icon");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_experienceLevelField)
{
    const QString experienceLevelField{"experienceLevelField"};
    json.changeField("json.experience_level", experienceLevelField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(experienceLevelField, offertData[0].experienceLevel);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_experienceLevelFieldIsNull)
{
    json.changeField("json.experience_level", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_experienceLevelFieldIsNumber)
{
    json.changeField("json.experience_level", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_experienceLevelFieldNoExist)
{
    json.removeField("json.experience_level");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_workplaceTypeField)
{
    const QString workplaceTypeField{"workplaceTypeField"};
    json.changeField("json.workplace_type", workplaceTypeField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(workplaceTypeField, offertData[0].workplace);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_workplaceTypeFieldIsNull)
{
    json.changeField("json.workplace_type", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_workplaceTypeFieldIsNumber)
{
    json.changeField("json.workplace_type", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_workplaceTypeFieldNoExist)
{
    json.removeField("json.workplace_type");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesArrayIsEmpty)
{
    json.changeField("json.employment_types", "[]", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesArrayIsNull)
{
    json.changeField("json.employment_types", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesArrayHaveBadType)
{
    json.changeField("json.employment_types", "employmentTypes");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesArrayNoExist)
{
    json.removeField("json.employment_types");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_employmentTypesSalaryFieldIsNull)
{
    constexpr int fieldQuantity = 1;
    json.addEmploymentTypesField(fieldQuantity);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    EXPECT_EQ(0, offertData[0].salary.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryFieldIsBadType)
{
    constexpr int fieldQuantity = 1;
    json.addEmploymentTypesField(fieldQuantity);
    json.changeField("json.employment_types.salary", "true", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryFieldNotExist)
{
    json.addEmploymentTypesField(1);
    json.removeField("json.employment_types.salary");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_employmentTypesSalaryTypeFieldExistButNoExistSalaryField)
{
    const QString typeField{"typeField"};
    json.addEmploymentTypesField(1);
    json.changeField("json.employment_types.type", typeField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(0, offertData[0].salary.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_employmentTypesSalaryTypeFieldExistAndSalaryFieldExist)
{
    const QString typeField{"typeField"};
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.type", typeField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(1, offertData[0].salary.length());
    EXPECT_EQ(typeField, offertData[0].salary[0].type);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryTypeFieldIsNull)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.type", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryTypeFieldIsBadType)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.type", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryTypeFieldNoExist)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.removeField("json.employment_types.type");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_employmentTypesSalaryCurrencyField)
{
    const QString currencyField{"currencyField"};
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.currency", currencyField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(1, offertData[0].salary.length());
    EXPECT_EQ(currencyField, offertData[0].salary[0].currency);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryCurrencyFieldIsNull)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.currency", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryCurrencyFieldIsBadType)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.currency", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryCurrencyFieldNoExist)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.removeField("json.employment_types.salary.currency");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_employmentTypesSalaryFromField)
{
    const quint32 fromFieldValue{100};
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.from", fromFieldValue);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(1, offertData[0].salary.length());
    EXPECT_EQ(fromFieldValue, offertData[0].salary[0].min);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryFromFieldIsNull)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.from", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryFromFieldIsBadType)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.from", "field");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryFromFieldNoExist)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.removeField("json.employment_types.salary.from");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_employmentTypesSalaryToField)
{
    const quint32 toFieldValue{100};
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.to", toFieldValue);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(1, offertData[0].salary.length());
    EXPECT_EQ(toFieldValue, offertData[0].salary[0].max);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryToFieldIsNull)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.to", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryToFieldIsBadType)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.changeField("json.employment_types.salary.to", "field");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_employmentTypesSalaryToFieldNoExist)
{
    json.addEmploymentTypesField(1);
    json.addEmploymentSalaryField();
    json.removeField("json.employment_types.salary.to");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayIsEmpty)
{
    json.changeField("json.skills", "[]", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayIsNull)
{
    json.changeField("json.skills", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayHaveBadType)
{
    json.changeField("json.skills", "skillsType");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayNoExist)
{
    json.removeField("json.skills");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_skillsArrayNameField)
{
    const QString skillsField{"skillsField"};
    json.addSkillsField(1);
    json.changeField("json.skills.name", skillsField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(1, offertData[0].skill.length());
    EXPECT_EQ(skillsField, offertData[0].skill[0].skillName);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayNameFieldIsNull)
{
    json.addSkillsField(1);
    json.changeField("json.skills.name", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayNameFieldIsBadType)
{
    json.addSkillsField(1);
    json.changeField("json.skills.name", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayNameFieldNoExist)
{
    json.addSkillsField(1);
    json.removeField("json.skills.name");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_skillsArrayLevelField)
{
    const quint8 levelField{3};
    json.addSkillsField(1);
    json.changeField("json.skills.level", levelField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(1, offertData[0].skill.length());
    EXPECT_EQ(levelField, offertData[0].skill[0].skillLevel);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayLevelFieldIsNull)
{
    json.addSkillsField(1);
    json.changeField("json.skills.level", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayLevelFieldIsBadType)
{
    json.addSkillsField(1);
    json.changeField("json.skills.level", "field");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_skillsArrayLevelFieldNoExist)
{
    json.addSkillsField(1);
    json.removeField("json.skills.level");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_multilocationArrayIsEmpty)
{
    json.changeField("json.multilocation", "[]", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_multilocationArrayIsNull)
{
    json.changeField("json.multilocation", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_multilocationArrayHaveBadType)
{
    json.changeField("json.multilocation", "multilocationType");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_multilocationArrayNoExist)
{
    json.removeField("json.multilocation");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_MultilocationArrayCityField)
{
    const QString cityField{"cityField"};
    json.addMultilocationField(1);
    json.changeField("json.multilocation.city", cityField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(1, offertData[0].multilocation.length());
    EXPECT_EQ(cityField, offertData[0].multilocation[0].city);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_MultilocationArrayCityFieldIsNull)
{
    json.addMultilocationField(1);
    json.changeField("json.multilocation.city", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_MultilocationArrayCityFieldIsBadType)
{
    json.addMultilocationField(1);
    json.changeField("json.multilocation.city", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_MultilocationArrayCityFieldNoExist)
{
    json.addMultilocationField(1);
    json.removeField("json.multilocation.city");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeSuccess_MultilocationArraySlugField)
{
    const QString slugField{"slugField"};
    json.addMultilocationField(1);
    json.changeField("json.multilocation.slug", slugField);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::SUCCESS, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(1, offertData.length());
    ASSERT_EQ(1, offertData[0].multilocation.length());
    EXPECT_EQ(slugField, offertData[0].multilocation[0].slugId);
}

TEST_F(JustJoinItDeserializerTest, deserializeError_MultilocationArraySlugFieldIsNull)
{
    json.addMultilocationField(1);
    json.changeField("json.multilocation.slug", "null", false);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    EXPECT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_MultilocationArraySlugFieldIsBadType)
{
    json.addMultilocationField(1);
    json.changeField("json.multilocation.slug", 0);
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}

TEST_F(JustJoinItDeserializerTest, deserializeError_MultilocationArraySlugFieldNoExist)
{
    json.addMultilocationField(1);
    json.removeField("json.multilocation.slug");
    json.makeValid();

    EXPECT_EQ(Error_Code_T::ERROR, deserializer.deserializeOffers(json.getRefJson(), offertData));
    ASSERT_EQ(0, offertData.length());
}
