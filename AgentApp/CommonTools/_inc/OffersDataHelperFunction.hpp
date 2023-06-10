#ifndef OFFERSDATAHELPERFUNCTION_HPP
#define OFFERSDATAHELPERFUNCTION_HPP

#include <QString>
#include <type_traits>

template<typename T, typename U>
typename std::enable_if<std::is_same<T, QString>::value, bool>::type
deserializeField(const U& obj, const QString fieldName, T& field)
{
    bool result = false;

    if(obj.contains(fieldName))
    {
        field = obj[fieldName].toString();
        result = true;
    }

    return result;
}

#endif // OFFERSDATAHELPERFUNCTION_HPP
