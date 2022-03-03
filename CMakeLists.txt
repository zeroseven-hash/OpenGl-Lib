cmake_minimum_required(VERSION 3.0.0)
project(example VERSION 0.1.0)

include(CTest)
enable_testing()

set(IncludePath "${CMAKE_CURRENT_SOURCE_DIR}/extern")


set(LibPath "${CMAKE_CURRENT_SOURCE_DIR}/Bin/Lib")



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Bin/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Bin/Debug)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/GLAD)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/opengl)

link_directories("${LibPath}")
add_executable(example main.cpp)

target_include_directories(example PRIVATE "${IncludePath}/GLAD/include" "${IncludePath}/glfw/include" "${IncludePath}/glm" "${IncludePath}/stb_image" "opengl/include")

target_link_libraries(example PRIVATE GLAD glfw3 opengl)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
