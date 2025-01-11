win32{
    INCLUDEPATH +=  $${SWW_LIBS_ROOT}/OpenEXR/include\
                    $${SWW_LIBS_ROOT}/OpenEXR/include/Imath

    CONFIG(debug, debug|release){
        LIBS +=\
        $${SWW_LIBS_ROOT}/OpenEXR/lib/Imath-3_2.lib\
#        $${SWW_LIBS_ROOT}/OpenEXR/lib/Half.lib\
        $${SWW_LIBS_ROOT}/OpenEXR/lib/Iex-3_2.lib\
        $${SWW_LIBS_ROOT}/OpenEXR/lib/IlmThread-3_2.lib
    }

    CONFIG(release, debug|release){
        LIBS +=\
        $${SWW_LIBS_ROOT}/OpenEXR/lib/Imath-3_2.lib\
#        $${SWW_LIBS_ROOT}/OpenEXR/lib/Half.lib\
        $${SWW_LIBS_ROOT}/OpenEXR/lib/Iex-3_2.lib\
        $${SWW_LIBS_ROOT}/OpenEXR/lib/IlmThread-3_2.lib
    }
}
