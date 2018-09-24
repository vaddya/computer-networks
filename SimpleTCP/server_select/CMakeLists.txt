cmake_minimum_required(VERSION 3.10)
project(server_select)

add_executable(server_select server_select.h server_select.cpp)

target_link_libraries(server_select pthread)