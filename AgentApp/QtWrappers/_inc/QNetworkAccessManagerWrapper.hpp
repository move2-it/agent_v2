#ifndef QNETWORKACCESSMANAGERWRAPPER_HPP
#define QNETWORKACCESSMANAGERWRAPPER_HPP

#include <QNetworkAccessManager>

class QNetworkAccessManagerWrapper
{
public:
    QNetworkAccessManagerWrapper() = default;

    ~QNetworkAccessManagerWrapper() = default;

    virtual QNetworkReply* get(const QNetworkRequest& request);

private:
    QNetworkAccessManager manager;
};

#endif // QNETWORKACCESSMANAGERWRAPPER_HPP
