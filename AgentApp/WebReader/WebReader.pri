QT += network

SOURCES += \
    $$PWD/_src/WebReader.cpp \
    $$PWD/_src/WebRequester.cpp

HEADERS += \
    $$PWD/_api/WebReaderApi.hpp \
    $$PWD/_inc/WebReader.hpp \
    $$PWD/_inc/WebReaderInterface.hpp \
    $$PWD/_inc/WebRequester.hpp \
    $$PWD/_inc/WebRequesterInterface.hpp

win32 {
    LIBS += -L$$PWD/../../3rdparty/OpenSSL/Win_x64
}
