#ifndef WEBREQUESTERINTERFACE_HPP
#define WEBREQUESTERINTERFACE_HPP

#include <QObject>
#include <QByteArray>

#include "CommonTypes.hpp"

class WebRequesterInterface : public QObject
{
    Q_OBJECT

public:

    WebRequesterInterface() = default;

    virtual ~WebRequesterInterface() = default;

    virtual Error_Code_T configureRequest(const QUrl& address) = 0;

    virtual QByteArray getRequestMessage() = 0;

public slots:
    virtual Error_Code_T execRequest() = 0;
};

#endif // WEBREQUESTERINTERFACE_HPP
