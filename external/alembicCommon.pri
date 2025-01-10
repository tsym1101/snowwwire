
win32{
#    DEFINES += PLATFORM_WINDOWS ALEMBIC_DLL #for AbcOpenGL
    INCLUDEPATH +=  $${SWW_LIBS_ROOT}/Alembic/include
    LIBS +=\
    $${SWW_LIBS_ROOT}/Alembic/lib/Alembic.lib

}
