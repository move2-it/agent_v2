#ifndef OFFERSDATA_HPP
#define OFFERSDATA_HPP

#include <QString>
#include <QList>

struct Salary
{
    QString type;
    QString currency;
    quint32 min;
    quint32 max;
};

struct Skill
{
    QString skillName;
    quint8 skillLevel;
};

struct OffertData
{
    QString idOffert;
    QString titleOffert;
    QString companyName;
    QString city;
    QString country;
    QString areaWork;
    QString experienceLevel;
    QList<Salary> salary;
    QList<Skill> skill;
    QString workplace;
};

#endif // OFFERSDATA_HPP
