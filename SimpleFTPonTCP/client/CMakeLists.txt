cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 11)

add_executable(client client.h client.cpp)

target_link_libraries(client utils)

target_include_directories(client PUBLIC ../utils)
