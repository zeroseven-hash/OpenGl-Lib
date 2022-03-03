cmake_minimum_required(VERSION 3.0.0)
project(GLAD VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Bin/Lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Bin/Lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(GLAD STATIC src/glad.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
