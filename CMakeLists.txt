cmake_minimum_required(VERSION 3.1.0)
project (NormalMapper VERSION 3.2.2)

set(CMAKE_INCLUDE_CURRENT_DIR ON) 

find_package(OpenGL REQUIRED)

if (WIN32)
    include(ExternalProject)
    ExternalProject_Add(freeglut
      GIT_REPOSITORY    https://github.com/dcnieho/FreeGLUT.git
      GIT_TAG           FG_3_0_0
      CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/freeglut
            -DFREEGLUT_BUILD_SHARED_LIBS=Off
            -DFREEGLUT_BUILD_STATIC_LIBS=On
            -DINSTALL_PDB=Off
      UPDATE_COMMAND ""
    )
    include_directories(${CMAKE_BINARY_DIR}/freeglut/include)
    link_directories(${CMAKE_BINARY_DIR}/freeglut/lib)

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(GLUT_LIBRARIES freeglut_staticd)
    else()
        set(GLUT_LIBRARIES freeglut_static)
    endif()
else ()
    find_package(GLUT REQUIRED)
endif (WIN32)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

set(SOURCES
    GL/glext.h
    Types.h
    Utils.h
    NMFConvert.cpp
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

set(SOURCES
    NMFConvert.cpp
    NormalMapper.rc
)
add_executable(NMFConvert ${SOURCES})
target_link_libraries (NMFConvert LINK_PUBLIC Common)

if (APPLE)
    set_source_files_properties(NativeDialogs.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa")
endif()

# Maya plugin...
#set(SOURCES
#    NmFileIO.cpp
#    NormalMapExport.cpp
#)
#add_executable(NMFExport ${SOURCES})

set(SOURCES
    NMFView.cpp
    NMFView.rc
)
add_executable(NMFView ${SOURCES})
if (WIN32)
    add_dependencies(NMFView freeglut)
    target_compile_definitions(NMFView PRIVATE FREEGLUT_LIB_PRAGMAS=0 FREEGLUT_STATIC=1)
endif()
target_link_libraries (NMFView LINK_PUBLIC Common ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(SOURCES
    NormalMapCombiner.cpp
    NormalMapper.rc
)
add_executable(NormalMapCombiner ${SOURCES})
target_link_libraries (NormalMapCombiner LINK_PUBLIC Common)

set(SOURCES
    AtiOctree.h
    AtiOctree.cpp
    AtiTriBoxMoller.h
    AtiTriBoxMoller.cpp
    NormalMapper.cpp
    NormalMapper.rc
)
add_executable(NormalMapper ${SOURCES})
target_link_libraries (NormalMapper LINK_PUBLIC Common)

set(SOURCES
    WorldSpaceifier.cpp
    NormalMapper.rc
)
add_executable(WorldSpaceifier ${SOURCES})
target_link_libraries (WorldSpaceifier LINK_PUBLIC Common)

set(SOURCES
    TGAtoDOT3.cpp
    NormalMapper.rc
)
add_executable(TGAtoDOT3 WIN32 ${SOURCES})
target_link_libraries (TGAtoDOT3 LINK_PUBLIC Common)

# Install
install(TARGETS NMFConvert DESTINATION bin)
#install(TARGETS NMFExport DESTINATION bin)
install(TARGETS NMFView DESTINATION bin)
install(TARGETS NormalMapCombiner DESTINATION bin)
install(TARGETS NormalMapper DESTINATION bin)
install(TARGETS WorldSpaceifier DESTINATION bin)
install(TARGETS TGAtoDOT3 DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data/ DESTINATION data)
install(FILES ${CMAKE_SOURCE_DIR}/doc/NormalMap.ppt DESTINATION doc)

# Package builder
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
