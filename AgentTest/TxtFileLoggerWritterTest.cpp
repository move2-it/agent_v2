#include <gtest/gtest.h>

#include "TxtFileLoggerWritter.hpp"
#include "MockQFileWrapper.hpp"

using ::testing::Return;

class TxtFileLoggerWritterTest : public ::testing::Test
{
public:
    TxtFileLoggerWritterTest() :
        txtFileWritter(fileWrapper)
    {

    }

protected:
    TxtFileLoggerWritter txtFileWritter;
    MockQFileWrapper fileWrapper;
    QList<OffertData> offertData{};
};

TEST_F(TxtFileLoggerWritterTest, CreateFileOpenFail)
{
    EXPECT_CALL(fileWrapper, open).Times(1).WillOnce(Return(false));
    EXPECT_CALL(fileWrapper, close).Times(0);

    EXPECT_EQ(Error_Code_T::ERROR, txtFileWritter.create());
}

TEST_F(TxtFileLoggerWritterTest, CreateFileSuccess)
{
    EXPECT_CALL(fileWrapper, open).Times(1).WillOnce(Return(true));
    EXPECT_CALL(fileWrapper, close).Times(1);

    EXPECT_EQ(Error_Code_T::SUCCESS, txtFileWritter.create());
}

TEST_F(TxtFileLoggerWritterTest, PrintFileOpenFail)
{
    EXPECT_CALL(fileWrapper, open).Times(1).WillOnce(Return(false));
    EXPECT_CALL(fileWrapper, write).Times(0);
    EXPECT_CALL(fileWrapper, close).Times(0);

    EXPECT_EQ(Error_Code_T::ERROR, txtFileWritter.print(offertData));
}

TEST_F(TxtFileLoggerWritterTest, PrintFileWriteDataTimeFail)
{
    EXPECT_CALL(fileWrapper, open).Times(1).WillOnce(Return(true));
    EXPECT_CALL(fileWrapper, write).Times(1).WillOnce(Return(-1));
    EXPECT_CALL(fileWrapper, close).Times(1);

    EXPECT_EQ(Error_Code_T::ERROR, txtFileWritter.print(offertData));
}

TEST_F(TxtFileLoggerWritterTest, PrintFileWriteOffertDataError)
{
    offertData.append(OffertData{});

    EXPECT_CALL(fileWrapper, open).Times(1).WillOnce(Return(true));
    EXPECT_CALL(fileWrapper, write).Times(2).WillOnce(Return(0)).WillOnce(Return(-1));
    EXPECT_CALL(fileWrapper, close).Times(1);

    EXPECT_EQ(Error_Code_T::ERROR, txtFileWritter.print(offertData));
}

TEST_F(TxtFileLoggerWritterTest, PrintFileWriteOneElementOffertDataSuccess)
{
    offertData.append(OffertData{});

    EXPECT_CALL(fileWrapper, open).Times(1).WillOnce(Return(true));
    EXPECT_CALL(fileWrapper, write).Times(2).WillOnce(Return(0)).WillOnce(Return(202));
    EXPECT_CALL(fileWrapper, close).Times(1);

    EXPECT_EQ(Error_Code_T::SUCCESS, txtFileWritter.print(offertData));
}

TEST_F(TxtFileLoggerWritterTest, PrintFileWriteTwoElementOffertDataSuccess)
{
    constexpr size_t size = 2;
    for(size_t i = 0; i < size; ++i)
    {
        offertData.append(OffertData{});
    }

    EXPECT_CALL(fileWrapper, open).Times(1).WillOnce(Return(true));
    EXPECT_CALL(fileWrapper, write).Times(size+1).WillOnce(Return(0)).WillRepeatedly(Return(202));
    EXPECT_CALL(fileWrapper, close).Times(1);

    EXPECT_EQ(Error_Code_T::SUCCESS, txtFileWritter.print(offertData));
}
