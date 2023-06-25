#include "LoggerInstantion.hpp"

static std::shared_ptr<MockLoggerWritterInterface> mockLoggerWriterInterface;

MockLoggerWritterInterface& getMockLoggerWrriterInterface()
{
    if(mockLoggerWriterInterface == nullptr)
    {
        mockLoggerWriterInterface = std::make_shared<MockLoggerWritterInterface>();
    }

    return *mockLoggerWriterInterface;
}

void clearMockLoggerWrriterInterface()
{
    mockLoggerWriterInterface.reset();
}

MockLoggerWritterInterface& getTxtFileLogger()
{
    return *mockLoggerWriterInterface;
}
