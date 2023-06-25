#include "PlatformInstantion.hpp"
#include "WebRequester.hpp"
#include "JustJoinItDeserializer.hpp"
#include "JustJoinItDataFilter.hpp"
#include "WebReader.hpp"

WebRequesterInterface& getWebRequester()
{
    static QNetworkAccessManagerWrapper networkAccessManager;
    static QEventLoopWrapper eventLoop;
    static WebRequester webRequester{networkAccessManager, eventLoop};
    return webRequester;
}

WebDeserializerInterface& getWebDeserializer()
{
    static JustJoinItDeserializer justJoinItDeserializer;
    return justJoinItDeserializer;
}

WebDataFilterInterface& getWebDataFilter()
{
    static JustJoinItDataFilter justJoinItDataFilter;
    return justJoinItDataFilter;
}

WebReaderInterface& getWebReader()
{
    static WebReader webReader{getWebRequester(), getWebDeserializer(), getWebDataFilter()};
    return webReader;
}
