#include <QNetworkReply>

#include "WebRequester.hpp"

WebRequester::WebRequester(QNetworkAccessManagerWrapper& _networkAccessManager,
                           QEventLoopWrapper& _eventLoop) :
    networkAccessManager(_networkAccessManager),
    eventLoop(_eventLoop)
{

}

Error_Code_T WebRequester::configureRequest(const QUrl& address)
{
    Error_Code_T result = Error_Code_T::ERROR;

    networkReply = std::make_unique<QNetworkReply*>(networkAccessManager.get(QNetworkRequest(address)));
    bool connectResult = connect(*networkReply.get(), &QNetworkReply::finished, &eventLoop, &QEventLoopWrapper::quit);

    if(connectResult == true)
    {
        result = Error_Code_T::SUCCESS;
        qDebug() << "WebRequester configured:" << address.toDisplayString();
    }
    else
    {
        qDebug() << "WebRequester configured error:" << address.toDisplayString();
    }

    return result;
}

Error_Code_T WebRequester::execRequest()
{
    Error_Code_T result = Error_Code_T::ERROR;

    int status = eventLoop.exec();
    if(status == static_cast<int>(QEventLoop::AllEvents))
    {
        result = readJsonResponse();
    }
    networkReply.reset();

    return result;
}

Error_Code_T WebRequester::readJsonResponse()
{
    Error_Code_T result = Error_Code_T::ERROR;

    if((*networkReply)->error() == QNetworkReply::NoError)
    {
        replyMessage = (*networkReply)->readAll();
        result = Error_Code_T::SUCCESS;
        qDebug() << "Read Web Page success";
    }
    else
    {
        qDebug() << "Execute reply message error - bad request";
    }

    return result;
}

QByteArray WebRequester::getRequestMessage()
{
    return std::move(replyMessage);
}
