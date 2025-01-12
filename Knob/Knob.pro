#-------------------------------------------------
#
# Project created by QtCreator 2015-08-30T17:26:10
#
#-------------------------------------------------

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

#include(../external/boostCommon.pri)
#include(../external/openimageioCommon.pri)

DEFINES += SWW_TEMP_BUILD

DEPENDPATH += $$PWD/../SwwCore\
              $$PWD/../SwwUtil

QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += SWW_TEMP_BUILD

TARGET = SwwKnob
TEMPLATE = lib

LIBS += -L$${DESTDIR} -lSwwCore\
        -L$${DESTDIR} -lSwwUtil\
        -L$${DESTDIR} -lSwwGeom

DEFINES += SWWKNOB_LIBRARY

#macx{
#PRE_TARGETDEPS += $${DESTDIR}/libSwwCore.dylib
#}

HEADERS += \
    KnobTypeDesc.h \
    KnobObject.h \
    KnobFloat.h \
    CustomSpinBox.h \
    KnobAPI.h \
    KnobBool.h \
    KnobInt.h \
    KnobVector3f.h \
    KnobVector4f.h \
    KnobMatrix44f.h \
    KnobExecButton.h \
    KnobFile.h \
    KnobEnum.h \
    KnobVector2i.h \
    KnobVector3i.h \
    KnobFloatSlider.h \
    KnobIntSlider.h \
    KnobMultiFile.h \
    KnobVector2f.h \
    KnobMultiLineEdit.h \
    KnobEntry.h \
    KnobMaker.h \
    CustomTextEdit.h \
    KnobFloatRange.h \
    FloatRangeSlider.h \
    KnobLineEdit.h \
    KnobMatrix33f.h \
    KnobBaseWidget.h \
    KnobGroupWidget.h \
    KnobEnumString.h \
    CommandStack.h \
    KnobTF.h \
    TransfuncMappingCanvas.h

SOURCES += \
    KnobObject.cpp \
    KnobFloat.cpp \
    CustomSpinBox.cpp \
    KnobBool.cpp \
    KnobInt.cpp \
    KnobVector3f.cpp \
    KnobVector4f.cpp \
    KnobMatrix44f.cpp \
    KnobExecButton.cpp \
    KnobFile.cpp \
    KnobEnum.cpp \
    KnobVector2i.cpp \
    KnobVector3i.cpp \
    KnobFloatSlider.cpp \
    KnobIntSlider.cpp \
    KnobMultiFile.cpp \
    KnobVector2f.cpp \
    KnobMultiLineEdit.cpp \
    KnobEntry.cpp \
    KnobMaker.cpp \
    CustomTextEdit.cpp \
    KnobFloatRange.cpp \
    FloatRangeSlider.cpp \
    KnobLineEdit.cpp \
    KnobMatrix33f.cpp \
    KnobBaseWidget.cpp \
    KnobGroupWidget.cpp \
    KnobEnumString.cpp \
    CommandStack.cpp \
    KnobTF.cpp \
    TransfuncMappingCanvas.cpp

FORMS += \
    KnobFloat.ui \
    KnobBool.ui \
    KnobInt.ui \
    KnobVector3f.ui \
    KnobVector4f.ui \
    KnobMatrix44f.ui \
    KnobExecButton.ui \
    KnobFile.ui \
    KnobEnum.ui \
    KnobVector2i.ui \
    KnobVector3i.ui \
    KnobFloatSlider.ui \
    KnobIntSlider.ui \
    KnobMultiFile.ui \
    KnobVector2f.ui \
    KnobMultiLineEdit.ui \
    KnobFloatRange.ui \
    KnobMatrix33f.ui \
    KnobBaseWidget.ui \
    KnobGroupWidget.ui \
    KnobEnumString.ui \
    KnobTF.ui

RESOURCES += \
    buttons.qrc


