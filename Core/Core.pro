#-------------------------------------------------
#
# Project created by QtCreator 2015-08-30T17:20:01
#
#-------------------------------------------------

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

QT       -= gui
QT += xml
TARGET = SwwCore
TEMPLATE = lib
DEFINES += SWWCORE_LIBRARY

SOURCES += \
    PlugBase.cpp \
    NodeRegister.cpp \
    Node.cpp \
    ConnectHandler.cpp \
    PlugTypeUtil.cpp \
    AutoUpdateLog.cpp \
    PlugType.cpp \
    SwwVersion.cpp \
    RootNode.cpp \
    JsonSerializer.cpp \
    JsonDeserializer.cpp \
    PlugRegister.cpp \
    Meta.cpp \
    Chainable.cpp \
    NodeBase.cpp \
#    Utils/IOUtility.cpp \
#    Utils/SequenceFile.cpp

HEADERS +=\
    PlugTypeDefFwd.h \
    SwwNsCore.h \
#    Utils/IOUtility.h \
#    Utils/SequenceFile.h \
#    Utils/SwwMath.h \
    VectorHelper.h \
    PlugType.h \
    PlugBase.h \
    Plug.h \
    NodeRegister.h \
    NodeBase.h \
    Node.h \
    SwwMacro.h \
    SwwAssert.h \
    ConnectHandler.h \
    CoreAPI.h \
    PlugTypeUtil.h \
    SwwTypeDesc.h \
    AutoUpdateLog.h \
    SwwTypeDef.h \
    SwwPluginInterface.h \
    SwwVersion.h \
    RootNode.h \
    DualMap.h \
    JsonSerializer.h \
    JsonDeserializer.h \
    SwwJson.h \
    PlugRegister.h \
    Meta.h \
    Chainable.h

