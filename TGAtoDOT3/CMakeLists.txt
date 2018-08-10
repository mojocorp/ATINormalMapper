
set(SOURCES
    TGAtoDOT3.cpp
    TGAtoDOT3.rc
)
add_executable(TGAtoDOT3 WIN32 ${SOURCES})
target_link_libraries (TGAtoDOT3 LINK_PUBLIC Common)

# Install
install(TARGETS TGAtoDOT3 DESTINATION bin)
