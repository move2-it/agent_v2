#include <algorithm>
#include <QDateTime>

#include "TxtFileLoggerWritter.hpp"

namespace {

const QString offertDataToStr(const OffertData& offert)
{
    static const QString formulaOffert = R"({
    idOffert: %1
    titleOffert: %2
    companyName: %3
    city: %4
    country: %5
    areaWork: %6
    experienceLevel: %7
    salary:
        %8
    skill:
        %9
    workplace: %10
    multilocation:
        %11
}
)";

    static const QString formulaSalary = R"({
            type: %1
            currency: %2
            min: %3
            max: %4
        })";

    static const QString formulaSkill = R"({
            skillName: %1
            skillLevel: %2
        })";

    static const QString formulaMultilocation = R"({
            city: %1
            slugId: %2
        })";

    QString salaryStr;
    std::for_each(offert.salary.cbegin(), offert.salary.cend(), [&](decltype(offert.salary)::value_type salary) {
        salaryStr.append(formulaSalary
                         .arg(salary.type)
                         .arg(salary.currency)
                         .arg(salary.min)
                         .arg(salary.max));
    });

    QString skillStr;
    std::for_each(offert.skill.cbegin(), offert.skill.cend(), [&](decltype(offert.skill)::value_type skill) {
        skillStr.append(formulaSkill
                         .arg(skill.skillName)
                         .arg(skill.skillLevel));
    });

    QString multilocationStr;
    std::for_each(offert.multilocation.cbegin(), offert.multilocation.cend(), [&](decltype(offert.multilocation)::value_type multilocation) {
        multilocationStr.append(formulaMultilocation
                         .arg(multilocation.city)
                         .arg(multilocation.slugId));
    });

    return QString(formulaOffert)
            .arg(offert.idOffert)
            .arg(offert.titleOffert)
            .arg(offert.companyName)
            .arg(offert.city)
            .arg(offert.country)
            .arg(offert.areaWork)
            .arg(offert.experienceLevel)
            .arg(salaryStr)
            .arg(skillStr)
            .arg(offert.workplace)
            .arg(multilocationStr);
}

} // end namespace

TxtFileLoggerWritter::TxtFileLoggerWritter(QFileWrapper& _fileWrapper) :
    fileWrapper(_fileWrapper)
{

}

Error_Code_T TxtFileLoggerWritter::create()
{
    Error_Code_T result = Error_Code_T::ERROR;

    if(fileWrapper.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        fileWrapper.close();
        result = Error_Code_T::SUCCESS;
    }

    return result;
}

Error_Code_T TxtFileLoggerWritter::print(const QList<OffertData>& offertData)
{
    Error_Code_T result = Error_Code_T::ERROR;

    if(fileWrapper.open(QIODevice::Append | QIODevice::Text))
    {
        QString logTime = QDateTime::currentDateTime().toString().append('\n');
        if(fileWrapper.write(logTime.toLocal8Bit().data()) != -1)
        {
            bool resultWrite = true;

            std::for_each(offertData.begin(), offertData.end(), [&](const OffertData offert) {
                const QString offertStr = offertDataToStr(offert);
                auto controlWrite = fileWrapper.write(offertStr.toLocal8Bit().data());
                if(controlWrite != offertStr.length())
                {
                    resultWrite = false;
                    return;
                }
            });

            if(resultWrite == true)
            {
                result = Error_Code_T::SUCCESS;
            }
        }
        fileWrapper.close();
    }

    return result;
}

