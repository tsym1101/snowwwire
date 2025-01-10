#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T12:23:23
#
#-------------------------------------------------

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

include(../external/openimageioCommon.pri)
#include(../external/proj4Common.pri)

#LIBS += -L$${DESTDIR} -lSwwLog

QT       -= gui

TARGET = SwwUtil
TEMPLATE = lib

DEFINES += SWWUTIL_LIBRARY

SOURCES += \
    IOUtility.cpp \
    SequenceFile.cpp \
#    Proj4Util.cpp

HEADERS += \
    SwwMath.h \
    SwwNsUtil.h \
    UtilAPI.h \
    IOUtility.h \
    SequenceFile.h \
#    Proj4Util.h


