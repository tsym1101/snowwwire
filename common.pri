#Includes common configuration for all subdirectory .pro files.

!exists(config.pri) {
  error("config.pri not found! copy config-default.pri to config.pri and edit!")
}
include(config.pri)

INCLUDEPATH += . ..
WARNINGS += -Wall
CONFIG += c++17
UI_DIR = uics
MOC_DIR = mocs
OBJECTS_DIR = objs

SW_HOME = $$PWD/../sww_build

macx{
DEFINES += SW_ENV_MAC
INCLUDEPATH += /opt/local/include
DESTDIR = "$${SW_HOME}/bin"

}
unix{
    DESTDIR = "$${SW_HOME}/bin"
}

win32{    
    QMAKE_CXXFLAGS += /FS #for jom.exe
    DEFINES += SW_ENV_WIN
    DEFINES += NOMINMAX

    CONFIG(debug, debug|release) {
        WIN32_DEBUG_NAME = debug
        DESTDIR = "$${SW_HOME}/bin/$${WIN32_DEBUG_NAME}"
    }
    CONFIG(release, debug|release) {
        WIN32_DEBUG_NAME = release
        DESTDIR = "$${SW_HOME}/bin/$${WIN32_DEBUG_NAME}"
    }
}

LIBS += -L$${DESTDIR} -lSwwLog

INCLUDEPATH += $${SWW_LIBS_ROOT}/nlohmann_json/include
include(external/ilmbaseCommon.pri)
