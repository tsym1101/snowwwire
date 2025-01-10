DEFINES += OPENVDB_USE_BLOSC
CONFIG += no_keywords
#DEFINES += "OPENVDB_ABI_VERSION_NUMBER=5"
#DEFINES += OPENVDB_DLL
#DEFINES += OPENVDB_REQUIRE_VERSION_NAME

win32{
QMAKE_CXXFLAGS += -bigobj
DEFINES += NOMINMAX

INCLUDEPATH +=  $${SWW_LIBS_ROOT}/OpenVDB/include

CONFIG(debug, debug|release) {
#    message( "isDebug" )
    LIBS +=\
    $${SWW_LIBS_ROOT}/OpenVDB/lib/openvdb.lib
}

CONFIG(release, debug|release) {
#    message( "isRelease" )
    LIBS +=\
    $${SWW_LIBS_ROOT}/OpenVDB/lib/openvdb.lib
}

}
