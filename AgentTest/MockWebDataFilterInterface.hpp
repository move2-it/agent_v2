#ifndef MOCKWEBDATAFILTERINTERFACE_HPP
#define MOCKWEBDATAFILTERINTERFACE_HPP

#include <gmock/gmock.h>
#include "WebDataFilterInterface.hpp"

class MockWebDataFilterInterface : public WebDataFilterInterface
{
public:
    MOCK_METHOD(Error_Code_T, filter, (QList<OffertData>& offertData), (override));
};

#endif // MOCKWEBDATAFILTERINTERFACE_HPP
