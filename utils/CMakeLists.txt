add_library(utils
        Stream.cpp
        Stream.h Alignment.cpp Alignment.h Color.cpp Color.h RectArray.cpp RectArray.h)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})