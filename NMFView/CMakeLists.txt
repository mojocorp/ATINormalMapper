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

# Install
install(TARGETS NMFView DESTINATION bin)
