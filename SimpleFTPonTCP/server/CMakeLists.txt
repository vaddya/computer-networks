cmake_minimum_required(VERSION 3.10)

project(server)

add_executable(${PROJECT_NAME} ftp_server.h ftp_server.cpp server.h server.cpp)

target_link_libraries(${PROJECT_NAME} pthread stdc++fs utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)