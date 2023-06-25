#ifndef JUSTJOINITDATAFILTER_HPP
#define JUSTJOINITDATAFILTER_HPP

#include "WebDataFilterInterface.hpp"

class JustJoinItDataFilter : public WebDataFilterInterface
{
public:
    JustJoinItDataFilter();

    virtual ~JustJoinItDataFilter() = default;

    virtual Error_Code_T filter(QList<OffertData>& offertData) override;
};

#endif // JUSTJOINITDATAFILTER_HPP
