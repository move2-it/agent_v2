#ifndef LOGGERWRITTERINTERFACE_H
#define LOGGERWRITTERINTERFACE_H

#include "CommonTypes.hpp"
#include "OffersData.hpp"

class LoggerWritterInterface
{
public:
    LoggerWritterInterface() = default;

    ~LoggerWritterInterface() = default;

    virtual Error_Code_T create() = 0;

    virtual Error_Code_T print(const QList<OffertData>& offertData) = 0;
};

#endif // LOGGERWRITTERINTERFACE_H
