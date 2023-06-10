QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += \
        main.cpp

INCLUDEPATH += \
    CommonTools/_inc \
    QtWrappers/_inc \
    WebReader/_inc \
    WebDeserializer/_inc \

include(CommonTools/CommonTools.pri)
include(QtWrappers/QtWrappers.pri)
include(WebReader/WebReader.pri)
include(WebDeserializer/WebDeserializer.pri)
