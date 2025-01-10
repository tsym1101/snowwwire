isEmpty( SWW_CUDA_VERSION ) {
    error("SWW_CUDA_VERSION not defined. eg SWW_CUDA_VERSION = v10.0")
}

message( "CUDA version : "  $${SWW_CUDA_VERSION} )

win32{

    CUDA_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/$${SWW_CUDA_VERSION}"
    CUDA_CC = "\"$${CUDA_DIR}/bin/nvcc.exe\""

    INCLUDEPATH += $$CUDA_DIR/include

    LIBS += -L$${CUDA_DIR}/lib/x64\
            -lcudart -lcuda

    CONFIG(debug,debug|release) {
        HOST_COMPILER_FLAG = "\"/EHsc  /nologo   /FS /Zi  /MDd \""
    }
    CONFIG(release,debug|release) {
        HOST_COMPILER_FLAG = "\"/EHsc  /nologo   /FS /Zi  /MD \""
    }

    cuda.name = "build by nvcc."
    cuda.output = ${QMAKE_FILE_BASE}.obj
    cuda.commands = $$CUDA_CC  -c --cudart static --machine 64 -Xcompiler $${HOST_COMPILER_FLAG}  -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
    cuda.input = CUDA_SOURCES
}

QMAKE_EXTRA_COMPILERS += cuda

#CUDA_SOURCESに追加する
#eg. CUDA_SOURCES = $$PWD/VectorAdd.cu
