#include <gtest/gtest.h>

#include "WebReader.hpp"
#include "MockWebRequesterInterface.hpp"
#include "MockWebDeserializerInterface.hpp"
#include "MockWebDataFilterInterface.hpp"
#include "LoggerInstantion.hpp"

using ::testing::Return;

class WebReaderTest : public ::testing::Test
{
public:
    WebReaderTest() :
        mockLogger(getMockLoggerWrriterInterface()),
        webReader{mockWebRequester, mockWebDeserializer, mockWebDataFilter}
    {

    }

    void TearDown() override
    {
        clearMockLoggerWrriterInterface();
    }

protected:
    MockWebRequesterInterface mockWebRequester;
    MockWebDeserializerInterface mockWebDeserializer;
    MockWebDataFilterInterface mockWebDataFilter;
    MockLoggerWritterInterface& mockLogger;
    WebReader webReader;
};

TEST_F(WebReaderTest, WebReaderCheckInitDefaultValues)
{
    EXPECT_EQ(Component_State_T::COMPONENT_NOT_INITIALISED, webReader.getState());
}

TEST_F(WebReaderTest, WebReaderInitSuccess)
{
    EXPECT_CALL(mockWebRequester, configureRequest).Times(1).WillOnce(Return(Error_Code_T::SUCCESS));

    EXPECT_EQ(Error_Code_T::SUCCESS, webReader.init());
    EXPECT_EQ(Component_State_T::COMPONENT_INITIALISED, webReader.getState());
}

TEST_F(WebReaderTest, WebReaderInitFailed)
{
    EXPECT_CALL(mockWebRequester, configureRequest).Times(1).WillOnce(Return(Error_Code_T::ERROR));

    EXPECT_EQ(Error_Code_T::ERROR, webReader.init());
    EXPECT_EQ(Component_State_T::COMPONENT_ERROR, webReader.getState());
}

TEST_F(WebReaderTest, WebReaderRunExecutionRequestError)
{
    EXPECT_CALL(mockWebRequester, execRequest).Times(1).WillOnce(Return(Error_Code_T::ERROR));
    EXPECT_CALL(mockWebDeserializer, deserializeOffers).Times(0);
    EXPECT_CALL(mockWebDataFilter, filter).Times(0);
    EXPECT_CALL(mockLogger, print).Times(0);

    EXPECT_EQ(Error_Code_T::ERROR, webReader.run());
    EXPECT_EQ(Component_State_T::COMPONENT_ERROR, webReader.getState());
}

TEST_F(WebReaderTest, WebReaderRunWebDeserializerError)
{
    EXPECT_CALL(mockWebRequester, execRequest).Times(1).WillOnce(Return(Error_Code_T::SUCCESS));
    EXPECT_CALL(mockWebDeserializer, deserializeOffers).Times(1).WillOnce(Return(Error_Code_T::ERROR));
    EXPECT_CALL(mockWebDataFilter, filter).Times(0);
    EXPECT_CALL(mockLogger, print).Times(0);

    EXPECT_EQ(Error_Code_T::ERROR, webReader.run());
    EXPECT_EQ(Component_State_T::COMPONENT_ERROR, webReader.getState());
}

TEST_F(WebReaderTest, WebReaderRunWebDataFilterError)
{
    EXPECT_CALL(mockWebRequester, execRequest).Times(1).WillOnce(Return(Error_Code_T::SUCCESS));
    EXPECT_CALL(mockWebDeserializer, deserializeOffers).Times(1).WillOnce(Return(Error_Code_T::SUCCESS));
    EXPECT_CALL(mockWebDataFilter, filter).Times(1).WillOnce(Return(Error_Code_T::ERROR));
    EXPECT_CALL(mockLogger, print).Times(0);

    EXPECT_EQ(Error_Code_T::ERROR, webReader.run());
    EXPECT_EQ(Component_State_T::COMPONENT_ERROR, webReader.getState());
}

TEST_F(WebReaderTest, WebReaderRunLoggerPrintError)
{
    EXPECT_CALL(mockWebRequester, execRequest).Times(1).WillOnce(Return(Error_Code_T::SUCCESS));
    EXPECT_CALL(mockWebDeserializer, deserializeOffers).Times(1).WillOnce(Return(Error_Code_T::SUCCESS));
    EXPECT_CALL(mockWebDataFilter, filter).Times(1).WillOnce(Return(Error_Code_T::SUCCESS));
    EXPECT_CALL(mockLogger, print).Times(1).WillOnce(Return(Error_Code_T::ERROR));

    EXPECT_EQ(Error_Code_T::ERROR, webReader.run());
    EXPECT_EQ(Component_State_T::COMPONENT_ERROR, webReader.getState());
}
