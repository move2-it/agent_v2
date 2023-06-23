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
        message("Use coverity flag for report generation")
        QMAKE_CXXFLAGS += -coverage -fprofile-arcs -ftest-coverage
        QMAKE_LFLAGS += -lgcov

        LIBS += -lgcov
    }
}

HEADERS += \
        MockQEventLoopWrapper.hpp \
        MockQNetworkAccessManagerWrapper.hpp \
        MockWebDeserializerInterface.hpp \
        MockWebRequesterInterface.hpp

SOURCES += \
        JustJoinItDeserializerTest.cpp \
        WebReaderTest.cpp \
        WebRequesterTest.cpp \
        main.cpp

INCLUDEPATH += \
        ../AgentApp/CommonTools/_inc \
        ../AgentApp/QtWrappers/_inc \
        ../AgentApp/WebDeserializer/_inc \
        ../AgentApp/WebReader/_inc

include(../AgentApp/CommonTools/CommonTools.pri)
include(../AgentApp/QtWrappers/QtWrappers.pri)
include(../AgentApp/WebDeserializer/WebDeserializer.pri)
include(../AgentApp/WebReader/WebReader.pri)
