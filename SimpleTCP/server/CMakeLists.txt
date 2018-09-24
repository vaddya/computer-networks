cmake_minimum_required(VERSION 3.10)
project(server)

add_executable(server server.h server.cpp)

target_link_libraries(server pthread utils)

target_include_directories(server PUBLIC ../utils)