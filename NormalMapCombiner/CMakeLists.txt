
set(SOURCES
    NormalMapCombiner.cpp
    NormalMapCombiner.rc
)
add_executable(NormalMapCombiner ${SOURCES})
target_link_libraries (NormalMapCombiner LINK_PUBLIC Common)

# Install
install(TARGETS NormalMapCombiner DESTINATION bin)

