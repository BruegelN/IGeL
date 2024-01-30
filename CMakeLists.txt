cmake_minimum_required(VERSION 3.16)
project(IGeL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(CLI11)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Release)

# Libigl
include(libigl)

# Enable the target igl::glfw
igl_include(glfw)
# Other modules you could enable
#igl_include(embree)
igl_include(imgui)
igl_include(opengl)
igl_include(stb)
#igl_include(predicates)
#igl_include(xml)
#igl_include(copyleft cgal)
#igl_include(copyleft comiso)
#igl_include(copyleft core)
#igl_include(copyleft cork)
#igl_include(copyleft tetgen)
#igl_include(restricted matlab)
#igl_include(restricted mosek)
#igl_include(restricted triangle)

add_executable(center src/center.cpp)
add_executable(view src/view.cpp)
add_executable(split src/split.cpp)
add_executable(to-stl src/to-stl.cpp)
add_executable(mesh_check src/mesh_check.cpp)
# add_executable(offset_surface src/offset_surface.cpp)
add_executable(colorize_boundaries src/colorize_boundaries.cpp)
add_executable(decimate src/decimate.cpp)

target_link_libraries(center igl::core)
target_link_libraries(view igl::core igl::opengl igl::glfw igl::imgui igl::stb)
target_link_libraries(split igl::core)
target_link_libraries(to-stl igl::core)
target_link_libraries(mesh_check igl::core)
# target_link_libraries(offset_surface igl::core)
target_link_libraries(colorize_boundaries igl::core igl::opengl igl::glfw igl::imgui igl::stb)
target_link_libraries(decimate igl::core)

install(TARGETS center;
                view;
                split;
                to-stl;
                mesh_check;
                # offset_surface;
                colorize_boundaries;
                decimate 
                RUNTIME DESTINATION "~/.local/bin")