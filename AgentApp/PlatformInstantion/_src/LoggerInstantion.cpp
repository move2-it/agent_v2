#include "LoggerInstantion.hpp"
#include "TxtFileLoggerWritter.hpp"
#include "QFileWrapper.hpp"

LoggerWritterInterface& getTxtFileLogger()
{
    static QFileWrapper fileWrapper("AgentApp.log");
    static TxtFileLoggerWritter txtFileLogger(fileWrapper);
    return txtFileLogger;
}
