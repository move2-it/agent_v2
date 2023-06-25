#include "PlatformInstantion.hpp"
#include "WebRequester.hpp"
#include "JustJoinItDeserializer.hpp"
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

WebReaderInterface& getWebReader()
{
    static WebReader webReader{getWebRequester(), getWebDeserializer()};
    return webReader;
}
