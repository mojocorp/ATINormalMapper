cmake_minimum_required(VERSION 3.1.0)
project (NormalMapper VERSION 3.2.2)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

add_subdirectory(Common)
add_subdirectory(NMFConvert)
add_subdirectory(NMFView)
add_subdirectory(NormalMapCombiner)
add_subdirectory(NormalMapper)
add_subdirectory(TGAtoDOT3)
add_subdirectory(WorldSpaceifier)

# Install
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data/ DESTINATION data)
install(FILES ${CMAKE_SOURCE_DIR}/doc/NormalMap.ppt DESTINATION doc)

# Package builder
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
