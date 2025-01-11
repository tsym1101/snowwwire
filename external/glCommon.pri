win32{
    greaterThan(QT_MAJOR_VERSION, 4): {
        LIBS += -lopengl32 -lglu32
    }

    INCLUDEPATH +=  $${SWW_LIBS_ROOT}/glew/include\
                    $${SWW_LIBS_ROOT}/glm/include

    LIBS +=\
    $${SWW_LIBS_ROOT}/glew/lib/glew32.lib

}

#macx{
#    LIBS +=  -L/opt/local/lib\
#             -lGLEW\
#             -lglut
#}
