include(gtest_dependency.pri)

QT += quick

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt

unix {
    DESTDIR = ../build/out/AgentTest

    OBJECTS_DIR = $$DESTDIR/.obj
    MOC_DIR = $$DESTDIR/.moc
}

HEADERS += \
        MockQEventLoopWrapper.hpp \
        MockQNetworkAccessManagerWrapper.hpp \
        MockWebDeserializerInterface.hpp \
        MockWebRequesterInterface.hpp

SOURCES += \
        OffersDataHelperFunctionTest.cpp \
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
