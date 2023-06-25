#include <QUrl>
#include <QDebug>

#include "WebReader.hpp"
#include "LoggerInstantion.hpp"
#include "PlatformCfg.hpp"

WebReader::WebReader(WebRequesterInterface& _webRequester,
                     WebDeserializerInterface& _webDeserializer) :
    componentState(Component_State_T::COMPONENT_NOT_INITIALISED),
    webRequester(_webRequester),
    webDeserializer(_webDeserializer)
{

}

Error_Code_T WebReader::init()
{
    Error_Code_T result = webRequester.configureRequest(QUrl("https://justjoin.it/api/offers"));

    if(result == Error_Code_T::SUCCESS)
    {
        componentState = Component_State_T::COMPONENT_INITIALISED;
        qDebug() << "WebReader initialize success";
    }
    else
    {
        componentState = Component_State_T::COMPONENT_ERROR;
        qDebug() << "WebReader initialize failed";
    }

    return result;
}

Error_Code_T WebReader::run()
{
    Error_Code_T result;

    do
    {
        result = webRequester.execRequest();
        if(result != Error_Code_T::SUCCESS)
        {
            qDebug() << "Request error";
            break;
        }

        QList<OffertData> offertData;
        result = webDeserializer.deserializeOffers(webRequester.getRequestMessage(), offertData);
        if(result != Error_Code_T::SUCCESS)
        {
            qDebug() << "Deserialize error";
            break;
        }

    #if FILE_LOGGER
        result = getTxtFileLogger().print(offertData);
        if(result != Error_Code_T::SUCCESS)
        {
            qDebug() << "File logging error";
            break;
        }
    #endif

    } while(false);

    if(result != Error_Code_T::SUCCESS)
    {
        componentState = Component_State_T::COMPONENT_ERROR;
    }

    return result;
}

Error_Code_T WebReader::deinit()
{
    return Error_Code_T::SUCCESS;
}

Component_State_T WebReader::getState()
{
    return componentState;
}
