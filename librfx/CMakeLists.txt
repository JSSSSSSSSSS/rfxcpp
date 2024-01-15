add_library(rfx
        Rfx.cpp
        Rfx.h
        RfxMessage.cpp
        RfxMessage.h
        DWT.cpp
        DWT.h
        Quantization.cpp
        Quantization.h
        RLGR.cpp
        RLGR.h)

target_include_directories(rfx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(rfx PUBLIC utils)