set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES
    GL/glext.h
    Types.h
    Utils.h
    NmFileIO.h
    NmFileIO.cpp
    TGAIO.h
    TGAIO.cpp
    ArgFileIO.h
    ArgFileIO.cpp
    NativeDialogs.h
    NativeDialogs.cpp
)
add_library(Common STATIC ${SOURCES})

target_include_directories (Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (APPLE)
    set_source_files_properties(NativeDialogs.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
    target_link_libraries(Common "-framework Cocoa")
endif()
