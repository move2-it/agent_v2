#ifndef WEBREQUESTER_HPP
#define WEBREQUESTER_HPP

#include <QJsonObject>

#include "WebRequesterInterface.hpp"
#include "QEventLoopWrapper.hpp"
#include "QNetworkAccessManagerWrapper.hpp"

class WebRequester : public WebRequesterInterface
{
public:
    explicit WebRequester(QNetworkAccessManagerWrapper& _networkAccessManager,
                          QEventLoopWrapper& _eventLoop);

    virtual Error_Code_T configureRequest(const QUrl& address) override;

    virtual Error_Code_T execRequest() override;

    virtual QByteArray getRequestMessage() override;

private:
    Error_Code_T readJsonResponse();

    QNetworkAccessManagerWrapper& networkAccessManager;
    QEventLoopWrapper& eventLoop;

    std::unique_ptr<QNetworkReply*> networkReply;
    QByteArray replyMessage;
};

#endif // WEBREQUESTER_HPP
