#ifndef COMMONTYPES_H
#define COMMONTYPES_H

enum Error_Code_T
{
    SUCCESS = 0,
    ERROR = 1,
    NULLPTR = 2,
    ZEROLENGTH = 3,
};

enum Component_State_T
{
    COMPONENT_NOT_INITIALISED = 0,
    COMPONENT_INITIALISED = 1,
    COMPONENT_WORKING = 2,
    COMPONENT_ERROR = 3,
};

#endif // COMMONTYPES_H
