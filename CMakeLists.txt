cmake_minimum_required(VERSION 3.0.0)
project(graphics-framework VERSION 0.0.1)

find_package(glfw3 3.3 REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

#Put output binary (and shaders) in /bin instead of /build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

add_executable(test ${SOURCES})

target_link_libraries(test glfw)

target_link_libraries(test OpenGL::GL)
target_link_libraries(test GLEW::GLEW)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/src/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders
)
add_dependencies(test copy_assets)