#include <gtest/gtest.h>
#include <QJsonObject>

#include "OffersDataHelperFunction.hpp"

using namespace testing;

TEST(OffersDataHelperFunctionTest, deserializeFieldQStringSuccess)
{
    constexpr char fieldName[] = "field";
    constexpr char fieldValue[] = "value";

    QJsonObject jsonObject;
    jsonObject[fieldName] = fieldValue;
    QString deserializeValue;

    EXPECT_EQ(true, deserializeField(jsonObject, fieldName, deserializeValue));
    EXPECT_EQ(fieldValue, deserializeValue);
}

TEST(OffersDataHelperFunctionTest, deserializeFieldQStringBadField)
{
    constexpr char fieldName[] = "field";
    constexpr char fieldValue[] = "value";

    QJsonObject jsonObject;
    jsonObject[fieldName] = fieldValue;
    QString deserializeValue{};

    EXPECT_EQ(false, deserializeField(jsonObject, "", deserializeValue));
    EXPECT_EQ(QString{}, deserializeValue);
}
