#include <QCoreApplication>
#include <QDebug>

#include "LoggerInstantion.hpp"
#include "PlatformInstantion.hpp"

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file}(%{line}): %{message}");
    getTxtFileLogger().create();

    QCoreApplication a(argc, argv);

    WebReaderInterface& webReader{getWebReader()};

    webReader.init();
    webReader.run();
    webReader.deinit();

    return a.exec();
}
