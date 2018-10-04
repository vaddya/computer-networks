cmake_minimum_required(VERSION 3.10)

project(client)

add_executable(${PROJECT_NAME} ftp_client.h ftp_client.cpp ftp_entity.h client.h client.cpp)

target_link_libraries(${PROJECT_NAME} pthread stdc++fs utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)