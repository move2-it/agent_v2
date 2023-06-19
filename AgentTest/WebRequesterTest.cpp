#include <gtest/gtest.h>
#include <QNetworkReply>

#include "MockQEventLoopWrapper.hpp"
#include "MockQNetworkAccessManagerWrapper.hpp"
#include "WebRequester.hpp"

using ::testing::Return;
using ::testing::NiceMock;

class WebRequesterTest : public ::testing::Test
{
public:
    WebRequesterTest() :
        webReader{mockAccessManager, mockEventLoop}
    {
    }

protected:
    MockQEventLoopWrapper mockEventLoop;
    NiceMock<MockQNetworkAccessManagerWrapper> mockAccessManager;
    WebRequester webReader;
};

class QNetworkReplyBridge : public QNetworkReply
{
public:
    QNetworkReplyBridge(QNetworkReply* originalReply, QObject* parent = nullptr)
        : QNetworkReply(parent)
    {
        setOperation(originalReply->operation());
        setRequest(originalReply->request());
        setUrl(originalReply->url());
        setHeader(QNetworkRequest::ContentLengthHeader, originalReply->header(QNetworkRequest::ContentLengthHeader));

        open(QIODevice::ReadOnly | QIODevice::Unbuffered);
    }

    void abort() override
    {
    }

    qint64 readData(char *data, qint64 maxSize) override
    {
        qint64 bytesToRead = qMin(maxSize, messageData.size() - messagePosition);

        if (bytesToRead > 0)
        {
            memcpy(data, messageData.constData() + messagePosition, bytesToRead);
            messagePosition += bytesToRead;
        }
        else
        {
            bytesToRead = -1;
        }

        return bytesToRead;
    }

    void setError(NetworkError errorCode, const QString &errorString)
    {
        QNetworkReply::setError(errorCode, errorString);
    }

    void setMessageDataToReadData(const QByteArray& message)
    {
        messageData = message;
        messagePosition = 0;
    }

private:
    QByteArray messageData{};
    qint64 messagePosition{0};
};

TEST_F(WebRequesterTest, WebRequesterCheckDefaultRequestMessage)
{
    EXPECT_EQ(QByteArray{}, webReader.getRequestMessage());
}

TEST_F(WebRequesterTest, WebRequesterConfigureSuccess)
{
    std::shared_ptr<QNetworkAccessManager> manager = std::make_shared<QNetworkAccessManager>();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl()));

    EXPECT_CALL(mockAccessManager, get).Times(1).WillOnce(Return(reply));

    EXPECT_EQ(Error_Code_T::SUCCESS, webReader.configureRequest(QUrl()));
}

TEST_F(WebRequesterTest, WebRequesterConfigureFail)
{
    EXPECT_CALL(mockAccessManager, get).Times(1).WillOnce(Return(nullptr));

    EXPECT_EQ(Error_Code_T::ERROR, webReader.configureRequest(QUrl()));
}

TEST_F(WebRequesterTest, WebRequesterExecutionSuccess)
{
    std::unique_ptr<QNetworkAccessManager> manager = std::make_unique<QNetworkAccessManager>();
    QNetworkReply *baseReply = manager->get(QNetworkRequest(QUrl()));
    std::unique_ptr<QNetworkReplyBridge> reply = std::make_unique<QNetworkReplyBridge>(baseReply);

    reply->setError(QNetworkReply::NetworkError::NoError, "NoError");
    reply->setMessageDataToReadData("messageData");

    ON_CALL(mockAccessManager, get).WillByDefault(Return(reply.get()));
    EXPECT_CALL(mockEventLoop, exec).Times(1).WillOnce(Return(QEventLoop::AllEvents));

    EXPECT_EQ(Error_Code_T::SUCCESS, webReader.configureRequest(QUrl()));
    EXPECT_EQ(Error_Code_T::SUCCESS, webReader.execRequest());
    EXPECT_STRNE("", webReader.getRequestMessage());
}

TEST_F(WebRequesterTest, WebRequesterExecutionEventFail)
{
    std::shared_ptr<QNetworkAccessManager> manager = std::make_shared<QNetworkAccessManager>();
    QNetworkReply *baseReply = manager->get(QNetworkRequest(QUrl()));
    std::unique_ptr<QNetworkReplyBridge> reply = std::make_unique<QNetworkReplyBridge>(baseReply);

    ON_CALL(mockAccessManager, get).WillByDefault(Return(reply.get()));
    EXPECT_CALL(mockEventLoop, exec).Times(1).WillOnce(Return(QEventLoop::X11ExcludeTimers));

    EXPECT_EQ(Error_Code_T::SUCCESS, webReader.configureRequest(QUrl()));
    EXPECT_EQ(Error_Code_T::ERROR, webReader.execRequest());
    EXPECT_STREQ("", webReader.getRequestMessage());
}

TEST_F(WebRequesterTest, WebRequesterExecutionNetworkReplyError)
{
    std::shared_ptr<QNetworkAccessManager> manager = std::make_shared<QNetworkAccessManager>();
    QNetworkReply *baseReply = manager->get(QNetworkRequest(QUrl()));
    std::unique_ptr<QNetworkReplyBridge> reply = std::make_unique<QNetworkReplyBridge>(baseReply);

    reply->setError(QNetworkReply::NetworkError::UnknownNetworkError, "UnknownNetworkError");

    ON_CALL(mockAccessManager, get).WillByDefault(Return(reply.get()));
    EXPECT_CALL(mockEventLoop, exec).Times(1).WillOnce(Return(QEventLoop::AllEvents));

    EXPECT_EQ(Error_Code_T::SUCCESS, webReader.configureRequest(QUrl()));
    EXPECT_EQ(Error_Code_T::ERROR, webReader.execRequest());
    EXPECT_STREQ("", webReader.getRequestMessage());
}

TEST_F(WebRequesterTest, WebRequesterExecutionSuccessMoveReadMessageCheck)
{
    std::shared_ptr<QNetworkAccessManager> manager = std::make_shared<QNetworkAccessManager>();
    QNetworkReply *baseReply = manager->get(QNetworkRequest(QUrl()));
    std::unique_ptr<QNetworkReplyBridge> reply = std::make_unique<QNetworkReplyBridge>(baseReply);

    reply->setError(QNetworkReply::NetworkError::NoError, "NoError");
    const QByteArray messageData{"messageData"};
    reply->setMessageDataToReadData(messageData);

    ON_CALL(mockAccessManager, get).WillByDefault(Return(reply.get()));
    EXPECT_CALL(mockEventLoop, exec).Times(1).WillOnce(Return(QEventLoop::AllEvents));

    EXPECT_EQ(Error_Code_T::SUCCESS, webReader.configureRequest(QUrl()));
    EXPECT_EQ(Error_Code_T::SUCCESS, webReader.execRequest());
    EXPECT_STREQ(messageData, webReader.getRequestMessage());
    EXPECT_STREQ("", webReader.getRequestMessage());
}
