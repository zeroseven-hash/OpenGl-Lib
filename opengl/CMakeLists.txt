cmake_minimum_required(VERSION 3.0.0)
project(opengl VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Bin/Lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Bin/Lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/extern/GLAD/include ${CMAKE_SOURCE_DIR}/extern/glm ${CMAKE_SOURCE_DIR}/extern/stb_image)


file(GLOB SRC "src/*.cpp" "./include/*.h")
add_library(opengl STATIC ${SRC})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
