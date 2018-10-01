cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} client.h client.cpp ftp_client.h ftp_client.cpp)

target_link_libraries(${PROJECT_NAME} utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)
