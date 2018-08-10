
set(SOURCES
    WorldSpaceifier.cpp
    WorldSpaceifier.rc
)
add_executable(WorldSpaceifier ${SOURCES})
target_link_libraries (WorldSpaceifier LINK_PUBLIC Common)

# Install
install(TARGETS WorldSpaceifier DESTINATION bin)
