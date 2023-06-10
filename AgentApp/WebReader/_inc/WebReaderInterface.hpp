#ifndef WEBREADERINTERFACE_HPP
#define WEBREADERINTERFACE_HPP

#include "CommonTypes.hpp"

class WebReaderInterface
{
public:
    WebReaderInterface() = default;

    virtual ~WebReaderInterface() = default;

    virtual Error_Code_T init() = 0;

    virtual Error_Code_T run() = 0;

    virtual Error_Code_T deinit() = 0;

    virtual Component_State_T getState() = 0;
};

#endif // WEBREADERINTERFACE_HPP
