#include <gtest/gtest.h>

#include "JustJoinItDataFilter.hpp"

class JustJoinItDataFilterTest : public ::testing::Test
{
public:
    JustJoinItDataFilterTest()
    {

    }

protected:
    JustJoinItDataFilter dataFilter;
    QList<OffertData> offertData{};
};

static Multilocation createMultilocation(QString id)
{
    return Multilocation {
        .city = "",
        .slugId = id,
    };
}

TEST_F(JustJoinItDataFilterTest, FilterEmptyList)
{
    EXPECT_EQ(Error_Code_T::ZEROLENGTH, dataFilter.filter(offertData));
}

TEST_F(JustJoinItDataFilterTest, FilterOneUniqueElementList)
{
    const QString expectId{"1"};
    OffertData offert;
    offert.idOffert = expectId,
    offert.multilocation = QList<Multilocation>{createMultilocation(expectId)};
    offertData.push_back(offert);

    EXPECT_EQ(Error_Code_T::SUCCESS, dataFilter.filter(offertData));
    ASSERT_EQ(offertData.length(), 1);
    EXPECT_EQ(offertData[0].idOffert, expectId);
}

TEST_F(JustJoinItDataFilterTest, FilterTwoUniqueElementList)
{
    const QString expectId_1{"1"};
    const QString expectId_2{"2"};

    OffertData offert_1;
    offert_1.idOffert = expectId_1,
    offert_1.multilocation = QList<Multilocation>{createMultilocation(expectId_1)};
    offertData.push_back(offert_1);

    OffertData offert_2;
    offert_2.idOffert = expectId_2,
    offert_2.multilocation = QList<Multilocation>{createMultilocation(expectId_2)};
    offertData.push_back(offert_2);

    EXPECT_EQ(Error_Code_T::SUCCESS, dataFilter.filter(offertData));
    ASSERT_EQ(offertData.length(), 2);
    EXPECT_EQ(offertData[0].idOffert, expectId_1);
    EXPECT_EQ(offertData[1].idOffert, expectId_2);
}

TEST_F(JustJoinItDataFilterTest, FilterFirstElementListWhenSecondShowFirst)
{
    const QString expectId_1{"1"};
    const QString expectId_2{"2"};

    OffertData offert_1;
    offert_1.idOffert = expectId_1,
    offert_1.multilocation = QList<Multilocation>{createMultilocation(expectId_1)};
    offertData.push_back(offert_1);

    OffertData offert_2;
    offert_2.idOffert = expectId_2,
    offert_2.multilocation = QList<Multilocation>{createMultilocation(expectId_2), createMultilocation(expectId_1)};
    offertData.push_back(offert_2);

    EXPECT_EQ(Error_Code_T::SUCCESS, dataFilter.filter(offertData));
    ASSERT_EQ(offertData.length(), 1);
    EXPECT_EQ(offertData[0].idOffert, expectId_2);
}

TEST_F(JustJoinItDataFilterTest, FilterSecondElementListWhenFirstShowSecond)
{
    const QString expectId_1{"1"};
    const QString expectId_2{"2"};

    OffertData offert_1;
    offert_1.idOffert = expectId_1,
    offert_1.multilocation = QList<Multilocation>{createMultilocation(expectId_1), createMultilocation(expectId_2)};
    offertData.push_back(offert_1);

    OffertData offert_2;
    offert_2.idOffert = expectId_2,
    offert_2.multilocation = QList<Multilocation>{createMultilocation(expectId_2)};
    offertData.push_back(offert_2);

    EXPECT_EQ(Error_Code_T::SUCCESS, dataFilter.filter(offertData));
    ASSERT_EQ(offertData.length(), 1);
    EXPECT_EQ(offertData[0].idOffert, expectId_1);
}
