
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

# Install
install(TARGETS NormalMapper DESTINATION bin)
