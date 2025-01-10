win32{
    INCLUDEPATH +=  $${SWW_LIBS_ROOT}/proj4/local/include

    CONFIG(debug, debug|release) {
        LIBS +=\
            -l$${SWW_LIBS_ROOT}/proj4/local/lib/proj_5_0_d
    }

    CONFIG(release, debug|release) {
         LIBS +=\
            -l$${SWW_LIBS_ROOT}/proj4/local/lib/proj_5_0
    }
}

