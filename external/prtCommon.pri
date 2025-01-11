win32{
    DEFINES += OPENEXR_DLL

    INCLUDEPATH +=  $${SWW_LIBS_ROOT}/PRT-IO-Library/include\
                    $${SWW_LIBS_ROOT}/zlib/include

    CONFIG(debug, debug|release) {
#        message( "isDebug" )
        LIBS +=  $${SWW_LIBS_ROOT}/zlib/lib/zlibd.lib
    }

    CONFIG(release, debug|release) {
#        message( "isRelease" )
        LIBS += $${SWW_LIBS_ROOT}/zlib/lib/zlib.lib
    }
}

