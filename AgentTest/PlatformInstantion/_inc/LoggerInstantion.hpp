#ifndef LOGGERINSTANTION_HPP
#define LOGGERINSTANTION_HPP

#include "MockLoggerWritterInterface.hpp"

MockLoggerWritterInterface& getMockLoggerWrriterInterface();
void clearMockLoggerWrriterInterface();
MockLoggerWritterInterface& getTxtFileLogger();

#endif // LOGGERINSTANTION_HPP
