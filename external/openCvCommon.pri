
win32{
    #DEFINES += OPENEXR_DLL

    INCLUDEPATH +=  $${SWW_LIBS_ROOT}/opencv/include

    CONFIG(debug, debug|release) {
    LIBS +=\
        $${SWW_LIBS_ROOT}/opencv/x64/vc17/lib/opencv_imgproc4100d.lib\
        $${SWW_LIBS_ROOT}/opencv/x64/vc17/lib/opencv_imgcodecs4100d.lib\
        $${SWW_LIBS_ROOT}/opencv/x64/vc17/lib/opencv_core4100d.lib\
        $${SWW_LIBS_ROOT}/opencv/x64/vc17/lib/opencv_highgui4100d.lib
    }

    CONFIG(release, debug|release) {
    LIBS +=\
        $${SWW_LIBS_ROOT}/opencv/x64/vc17/lib/opencv_imgproc4100.lib\
        $${SWW_LIBS_ROOT}/opencv/x64/vc17/lib/opencv_imgcodecs4100.lib\
        $${SWW_LIBS_ROOT}/opencv/x64/vc17/lib/opencv_core4100.lib\
        $${SWW_LIBS_ROOT}/opencv/x64/vc17/lib/opencv_highgui4100.lib
    }


}
