cmake_minimum_required(VERSION 3.10)

project(SimpleFTPonUDP)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(utils)

add_custom_target(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} server client)
