#ifndef WEBREADER_H
#define WEBREADER_H

#include "WebReaderInterface.hpp"
#include "WebRequesterInterface.hpp"
#include "WebDeserializerInterface.hpp"

class WebReader : public WebReaderInterface
{
public:
    explicit WebReader(WebRequesterInterface& _webRequester,
                       WebDeserializerInterface& _webDeserializer);

    virtual ~WebReader() = default;

    virtual Error_Code_T init() override;

    virtual Error_Code_T run() override;

    virtual Error_Code_T deinit() override;

    virtual Component_State_T getState() override;

private:
    Component_State_T componentState;

    WebRequesterInterface& webRequester;
    WebDeserializerInterface& webDeserializer;
};

#endif // WEBREADER_H
