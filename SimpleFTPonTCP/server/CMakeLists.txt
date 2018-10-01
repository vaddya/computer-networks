cmake_minimum_required(VERSION 3.10)
project(server)

add_executable(${PROJECT_NAME} server.h server.cpp ftp_server.h ftp_server.cpp)

target_link_libraries(${PROJECT_NAME} pthread utils stdc++fs)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)