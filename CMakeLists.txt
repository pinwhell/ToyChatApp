cmake_minimum_required(VERSION 3.15)

project(ChatApp)

include_directories(include)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)