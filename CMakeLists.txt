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
    NMFConvert.cpp
    NmFileIO.cpp
    NormalMapper.rc
)
add_executable(NMFConvert ${SOURCES})

# Maya plugin...
#set(SOURCES
#    NmFileIO.cpp
#    NormalMapExport.cpp
#)
#add_executable(NMFExport ${SOURCES})

set(SOURCES
    NmFileIO.cpp
    NMFView.cpp
    NMFView.rc
    SMDFileIO.cpp
    TGAIO.cpp
)
add_executable(NMFView ${SOURCES})
if (WIN32)
    add_dependencies(NMFView freeglut)
    target_compile_definitions(NMFView PRIVATE FREEGLUT_LIB_PRAGMAS=0 FREEGLUT_STATIC=1)
endif()
target_link_libraries (NMFView LINK_PUBLIC ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

set(SOURCES
    ArgFileIO.cpp
    NormalMapCombiner.cpp
    NormalMapper.rc
    TGAIO.cpp
)
add_executable(NormalMapCombiner ${SOURCES})

set(SOURCES
    ArgFileIO.cpp
    AtiOctree.cpp
    AtiTriBoxMoller.cpp
    NmFileIO.cpp
    NormalMapper.cpp
    NormalMapper.rc
    SMDFileIO.cpp
    TGAIO.cpp
)
add_executable(NormalMapper ${SOURCES})

set(SOURCES
    ArgFileIO.cpp
    NmFileIO.cpp
    SMDFileIO.cpp
    TGAIO.cpp
    WorldSpaceifier.cpp
    NormalMapper.rc
)
add_executable(WorldSpaceifier ${SOURCES})

# Install
install(TARGETS NMFConvert DESTINATION bin)
#install(TARGETS NMFExport DESTINATION bin)
install(TARGETS NMFView DESTINATION bin)
install(TARGETS NormalMapCombiner DESTINATION bin)
install(TARGETS NormalMapper DESTINATION bin)
install(TARGETS WorldSpaceifier DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data/ DESTINATION data)

# Package builder
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
