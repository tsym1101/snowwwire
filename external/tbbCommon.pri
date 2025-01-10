win32{
    INCLUDEPATH +=  $${SWW_LIBS_ROOT}/TBB/include
    LIBS +=\
        $${SWW_LIBS_ROOT}/TBB/lib/tbb.lib\
        $${SWW_LIBS_ROOT}/TBB/lib/tbb12.lib\
        $${SWW_LIBS_ROOT}/TBB/lib/tbbmalloc.lib

    #for avoiding linking tbb12_debug.lib
    DEFINES += __TBB_NO_IMPLICIT_LINKAGE
}

macx{
     error( "tbbCommon not set on OSX." )
}
