#include <QCoreApplication>
#include <QDebug>

#include "WebReader.hpp"
#include "WebRequester.hpp"
#include "JustJoinItDeserializer.hpp"

#include "QNetworkAccessManagerWrapper.hpp"
#include "QEventLoopWrapper.hpp"

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

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file}(%{line}): %{message}");

    QCoreApplication a(argc, argv);

    WebReaderInterface& webReader{getWebReader()};

    webReader.init();
    webReader.run();
    webReader.deinit();

    return a.exec();
}
