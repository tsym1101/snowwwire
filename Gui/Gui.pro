#-------------------------------------------------
#
# Project created by QtCreator 2015-08-30T17:26:10
#
#-------------------------------------------------

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

include(../external/glCommon.pri)
#include(../external/openimageiocommon.pri)

DEPENDPATH += $$PWD/../Core/\
              $$PWD/../Knob/

DEFINES += SWW_TEMP_BUILD

QT       += core gui xml opengl \
    widgets
greaterThan(QT_MAJOR_VERSION, 4): {
    QT       += concurrent
}

TARGET = SwwGui
TEMPLATE = lib

LIBS += -L$${DESTDIR}  -lSwwUtil -lSwwCore -lSwwGeom -lSwwKnob -lSwwOpenGL

DEFINES += SWWGUI_LIBRARY

#macx{
#PRE_TARGETDEPS += $${DESTDIR}/libSwwCore.dylib\
#            $${DESTDIR}/libSwwKnob.dylib
#}

SOURCES += \
    PlugItem.cpp \
    NodeMaker.cpp \
    NodeItem.cpp \
    NodeGraphScene.cpp \
    Connector.cpp \
    KnobMaker.cpp \
    KnobWidget.cpp \
    NodeGraphWidget.cpp \
    MainWindow.cpp \
    ConnectHandlerGui.cpp \
    TextRenderer.cpp \
    TimeLine.cpp \
    TimeLineUtils.cpp \
    TimeLineWidget.cpp \
    TimeSlider.cpp \
    ProgressWidget.cpp \
    GuiGlobal.cpp \
    AddDynamicPlugWidget.cpp \
    ConnectDialog.cpp \
    Command.cpp

HEADERS +=\
    PlugItem.h \
    NodeMaker.h \
    NodeItem.h \
    NodeGraphScene.h \
    Connector.h \
    KnobMaker.h \
    KnobWidget.h \
    NodeGraphWidget.h \
    MainWindow.h \
    ConnectHandlerGui.h \
    GuiAPI.h \
    TextRenderer.h \
    TimeLine.h \
    TimeLineUtils.h \
    TimeLineWidget.h \
    TimeSlider.h \
    ProgressWidget.h \
    GuiGlobal.h \
    AddDynamicPlugWidget.h \
    ConnectDialog.h \
    Command.h

#symbian {
#    MMP_RULES += EXPORTUNFROZEN
#    TARGET.UID3 = 0xE1EC7B30
#    TARGET.CAPABILITY =
#    TARGET.EPOCALLOWDLLDATA = 1
#    addFiles.sources = SwwGui.dll
#    addFiles.path = !:/sys/bin
#    DEPLOYMENT += addFiles
#}

#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}

FORMS += \
    KnobWidget.ui \
    MainWindow.ui \
    TimeSlider.ui \
    ProgressWidget.ui \
    AddDynamicPlugWidget.ui \
    ConnectDialog.ui

RESOURCES += \
    style.qrc

OTHER_FILES += \
    Style_MainWindow.css
