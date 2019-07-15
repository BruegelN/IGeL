cmake_minimum_required(VERSION 3.1)
project(IGeL)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# libigl
option(BUILD_SHARED_LIBS             "build shared libraries" ON)
option(LIBIGL_USE_STATIC_LIBRARY     "Use libIGL as static librarie" OFF)
option(LIBIGL_BUILD_PYTHON           "Build libigl python bindings" OFF)
option(LIBIGL_WITH_PYTHON            "Use Python"         OFF)
option(LIBIGL_WITH_CGAL              "Use CGAL"           ON)
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_OPENGL_GLFW_IMGUI       "Use ImGUI"           ON)

find_package(LIBIGL REQUIRED QUIET)

set(EXECUTABLE_OUTPUT_PATH "bin")

add_executable(offset_surface src/offset_surface.cpp)
add_executable(center src/center.cpp)
add_executable(view src/view.cpp)
target_link_libraries(offset_surface igl::core igl::cgal)
target_link_libraries(center igl::core)
target_link_libraries(view igl::core igl::opengl igl::opengl_glfw igl::opengl_glfw_imgui)
