cmake_minimum_required(VERSION 3.1)
project(IGeL)

include_directories(CLI11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Release)

# libigl
option(LIBIGL_BUILD_PYTHON           "Build libigl python bindings" OFF)
option(LIBIGL_WITH_PYTHON            "Use Python"         OFF)
option(LIBIGL_WITH_PNG               "Use PNG"            ON)
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_OPENGL_GLFW_IMGUI       "Use ImGUI"           ON)

find_package(LIBIGL REQUIRED QUIET)

set(EXECUTABLE_OUTPUT_PATH "bin")

add_executable(center src/center.cpp)
add_executable(view src/view.cpp)
add_executable(split src/split.cpp)
add_executable(to-stl src/to-stl.cpp)
add_executable(mesh_check src/mesh_check.cpp)
add_executable(offset_surface src/offset_surface.cpp)
add_executable(colorize_boundaries src/colorize_boundaries.cpp)
add_executable(decimate src/decimate.cpp)

target_link_libraries(center igl::core)
target_link_libraries(view igl::core igl::opengl igl::opengl_glfw igl::opengl_glfw_imgui igl::png)
target_link_libraries(split igl::core)
target_link_libraries(to-stl igl::core)
target_link_libraries(mesh_check igl::core)
target_link_libraries(offset_surface igl::core)
target_link_libraries(colorize_boundaries igl::core igl::opengl igl::opengl_glfw igl::opengl_glfw_imgui igl::png)
target_link_libraries(decimate igl::core)
