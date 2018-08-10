set(SOURCES
    NMFConvert.cpp
    NMFConvert.rc
)
add_executable(NMFConvert ${SOURCES})
target_link_libraries (NMFConvert LINK_PUBLIC Common)

# Install
install(TARGETS NMFConvert DESTINATION bin)



