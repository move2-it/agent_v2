include(gtest_dependency.pri)

QT += quick

TEMPLATE = app
CONFIG += c++17 console
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt

unix {
    DESTDIR = ../build/out/AgentTest

    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc

    equals(GCOVERAGE, true) {
        message("Use GCOVERAGE flag for test coverage raport generation")
        QMAKE_CXXFLAGS += -coverage -fprofile-arcs -ftest-coverage
        QMAKE_LFLAGS += -lgcov

        LIBS += -lgcov
    }
}

HEADERS += \
        MockLoggerWritterInterface.hpp \
        MockQEventLoopWrapper.hpp \
        MockQFileWrapper.hpp \
        MockQNetworkAccessManagerWrapper.hpp \
        MockWebDataFilterInterface.hpp \
        MockWebDeserializerInterface.hpp \
        MockWebRequesterInterface.hpp

SOURCES += \
        JustJoinItDataFilterTest.cpp \
        JustJoinItDeserializerTest.cpp \
        TxtFileLoggerWritterTest.cpp \
        WebReaderTest.cpp \
        WebRequesterTest.cpp \
        main.cpp

INCLUDEPATH += \
        PlatformCfg/_inc \
        PlatformInstantion/_inc \
        ../AgentApp/CommonTools/_inc \
        ../AgentApp/QtWrappers/_inc \
        ../AgentApp/WebDeserializer/_inc \
        ../AgentApp/WebReader/_inc \
        ../AgentApp/LoggerWritter/_inc \
        ../AgentApp/WebDataFilter/_inc \

include(PlatformCfg/PlatformCfg.pri)
include(PlatformInstantion/PlatformInstantion.pri)
include(../AgentApp/CommonTools/CommonTools.pri)
include(../AgentApp/QtWrappers/QtWrappers.pri)
include(../AgentApp/WebDeserializer/WebDeserializer.pri)
include(../AgentApp/WebReader/WebReader.pri)
include(../AgentApp/LoggerWritter/LoggerWritter.pri)
include(../AgentApp/WebDataFilter/WebDataFilter.pri)
