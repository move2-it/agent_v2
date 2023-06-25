#ifndef WEBDATAFILTERINTERFACE_HPP
#define WEBDATAFILTERINTERFACE_HPP

#include "CommonTypes.hpp"
#include "OffersData.hpp"

class WebDataFilterInterface
{
public:
    WebDataFilterInterface() = default;

    virtual ~WebDataFilterInterface() = default;

    virtual Error_Code_T filter(QList<OffertData>& offertData) = 0;
};

#endif // WEBDATAFILTERINTERFACE_HPP
