#ifndef TXTFILELOGGERWRITTER_H
#define TXTFILELOGGERWRITTER_H

#include "LoggerWritterInterface.hpp"
#include "QFileWrapper.hpp"

class TxtFileLoggerWritter : public LoggerWritterInterface
{
public:
    explicit TxtFileLoggerWritter(QFileWrapper& _fileWrapper);

    virtual ~TxtFileLoggerWritter() = default;

    virtual Error_Code_T create() override;

    virtual Error_Code_T print(const QList<OffertData>& offertData) override;

private:
    QFileWrapper& fileWrapper;
};

#endif // TXTFILELOGGERWRITTER_H
