cmake_minimum_required(VERSION 3.0.0)
project(example VERSION 0.1.0)

include(CTest)
enable_testing()



set(IncludePath PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/extern")
set(LibPath PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Bin/Lib")

include_directories("${IncludePath}/GLAD/include" "${IncludePath}/glfw/include" "${IncludePath}/glm" "${IncludePath}/stb_image" "opengl/include")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Bin/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Bin/Debug)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/GLAD)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/opengl)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/example)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
