cmake_minimum_required(VERSION 2.6)
project(MandelbrotZoom)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${MandelbrotZoom_SOURCE_DIR}/cmake_scripts")

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
find_package(SDL2 REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 --pedantic-errors -Wall -Wextra")
# include_directories(${GLM_INCLUDE_DIRS})

add_executable(MandelbrotZoom main.cpp)
set(LIBRARIES ${SDL2_LIBRARY} ${OPENGL_LIBRARY} ${CMAKE_DL_LIBS})
target_link_libraries(MandelbrotZoom ${LIBRARIES})

install(TARGETS MandelbrotZoom RUNTIME DESTINATION bin)
