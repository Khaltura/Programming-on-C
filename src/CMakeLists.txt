cmake_minimum_required(VERSION 3.6.0)
add_library(IPParser INTERFACE IPParser.hpp)
add_library(IPStruct IPStruct.hpp IPStruct.cpp)
add_executable(LAB2 main.cpp)

target_link_libraries(LAB2 IPParser IPStruct)