cmake_minimum_required(VERSION 2.6)
project(mandelbrotzoom)

add_executable(mandelbrotzoom main.cpp)

install(TARGETS mandelbrotzoom RUNTIME DESTINATION bin)
