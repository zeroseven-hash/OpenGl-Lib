cmake_minimum_required(VERSION 3.0.0)
project(texture VERSION 0.1.0)


link_directories("${LibPath}")
add_executable(instanced instanced.cpp)

target_link_libraries(instanced PUBLIC GLAD glfw3 opengl)