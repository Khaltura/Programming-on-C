cmake_minimum_required(VERSION 3.5.0)
project(finder VERSION 0.1.0 LANGUAGES C CXX)
find_package(Boost 1.82 REQUIRED COMPONENTS filesystem program_options)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(finder main.cpp)
    target_link_libraries(finder ${Boost_LIBRARIES})
endif()
