add_library(rfx
        Rfx.cpp
        Rfx.h
        RfxMessage.cpp
        RfxMessage.h
        Dwt.cpp
        Dwt.h
        Quantization.cpp
        Quantization.h
        Rlgr.cpp
        Rlgr.h Tile.h Tile.cpp Linearization.cpp Linearization.h)

target_include_directories(rfx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(rfx PUBLIC utils)
target_link_libraries(rfx PUBLIC bmp)