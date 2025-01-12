#-------------------------------------------------
#
# Project created by QtCreator 2015-08-30T17:30:58
#
#-------------------------------------------------

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

include(../external/glCommon.pri)
#include(../external/tbbCommon.pri)
#include(../external/openimageioCommon.pri)

CONFIG   += console

DEFINES += SWW_TEMP_BUILD

DEPENDPATH += $$PWD/../Util/\
              $$PWD/../Core/\
              $$PWD/../Knob/\
              $$PWD/../Gui/\
#              $$PWD/../Node/\
              $$PWD/../OpenGL/\
              $$PWD/../plugins/

QT       += core gui opengl
CONFIG -= app_bundle

TARGET = snowwwire
TEMPLATE = app

LIBS += -L$${DESTDIR}  -lSwwUtil -lSwwCore -lSwwGeom -lSwwKnob -lSwwGui -lSwwOpenGL

SOURCES += main.cpp \
    Entry.cpp \
    InitHandler.cpp \
    OptionParser.cpp \
    App.cpp \
    UnitTest.cpp

HEADERS  += \
    Entry.h \
    InitHandler.h \
    OptionParser.h \
    App.h \
    UnitTest.h

FORMS    +=

RC_FILE = ico.rc
