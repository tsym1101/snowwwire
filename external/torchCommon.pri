win32{
    DEFINES += NOMINMAX

    INCLUDEPATH += $${SWW_LIBS_ROOT}/CURL/include\
    $${SWW_LIBS_ROOT}/zlib/include

    LIBS += $${SWW_LIBS_ROOT}/CURL/lib/libcurl_imp.lib\
    $${SWW_LIBS_ROOT}/zlib/lib/zlib.lib


#    INCLUDEPATH += $${SWW_CUDNN_ROOT}/include
#    LIBS += -L$${SWW_CUDNN_ROOT}/lib/x64

    CONFIG(debug, debug|release){
         SWW_TORCH_ROOT = $${SWW_LIBS_ROOT}/_prebuild/libtorch-win-shared-with-deps-debug-2.4.1+cu118/libtorch
    }
    CONFIG(release, debug|release){
         SWW_TORCH_ROOT = $${SWW_LIBS_ROOT}/_prebuild/libtorch-win-shared-with-deps-2.4.0+cu118/libtorch
    }

    SWW_CUDNN_ROOT = $${SWW_LIBS_ROOT}/_prebuild/cudnn-windows-x86_64-9.3.0.75_cuda11-archive


    INCLUDEPATH += $${SWW_TORCH_ROOT}/include/torch/csrc/api/include\
    $${SWW_TORCH_ROOT}/include
    LIBS += $${SWW_TORCH_ROOT}/lib/torch.lib\
    $${SWW_TORCH_ROOT}/lib/c10.lib\
    $${SWW_TORCH_ROOT}/lib/torch_cpu.lib\
    $${SWW_TORCH_ROOT}/lib/torch_cuda.lib
}
