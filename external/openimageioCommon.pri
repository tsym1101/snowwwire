
win32{
    INCLUDEPATH +=  \
                    $${SWW_LIBS_ROOT}/OpenImageIO/include

             CONFIG(debug, debug|release){
             LIBS +=\
                     $${SWW_LIBS_ROOT}/OpenImageIO/lib/OpenImageIO_d.lib\
                     $${SWW_LIBS_ROOT}/OpenImageIO/lib/OpenImageIO_Util_d.lib
             }
              CONFIG(release, debug|release){
              LIBS +=\
                      $${SWW_LIBS_ROOT}/OpenImageIO/lib/OpenImageIO.lib\
                      $${SWW_LIBS_ROOT}/OpenImageIO/lib/OpenImageIO_Util.lib
              }
}
