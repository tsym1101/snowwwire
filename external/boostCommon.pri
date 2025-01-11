win32{
    INCLUDEPATH += $${SWW_LIBS_ROOT}/boost/include/boost-1_82
    LIBS += -L$${SWW_LIBS_ROOT}/boost/lib
    CONFIG(debug, debug|release){
    }
    CONFIG(release, debug|release){        
    }     
}
