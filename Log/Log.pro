
##from common.pri ###########
INCLUDEPATH += . ..
WARNINGS += -Wall

QT +=  core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs

Sww_HOME = $$PWD/../../Sww_build

macx{
DEFINES += Sww_ENV_MAC
INCLUDEPATH += /opt/local/include
DESTDIR = "$${Sww_HOME}/bin"

}
unix{
    DESTDIR = "$${Sww_HOME}/bin"
}

win32{
    QMAKE_CXXFLAGS += /FS #for jom.exe
    DEFINES += Sww_ENV_WIN
    DEFINES += NOMINMAX

    CONFIG(debug, debug|release) {
        WIN32_DEBUG_NAME = debug
        DESTDIR = "$${Sww_HOME}/bin/$${WIN32_DEBUG_NAME}"
    }
    CONFIG(release, debug|release) {
        WIN32_DEBUG_NAME = release
        DESTDIR = "$${Sww_HOME}/bin/$${WIN32_DEBUG_NAME}"
    }
}
#############################

TARGET = SwwLog
TEMPLATE = lib

DEFINES += SWWLOG_LIBRARY

SOURCES += Log.cpp \
    ConsoleWidget.cpp

HEADERS += Log.h\
    ConsoleWidget.h \
    logApi.h

FORMS += \
    ConsoleWidget.ui

