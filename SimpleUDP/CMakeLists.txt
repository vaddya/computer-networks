cmake_minimum_required(VERSION 3.10)

project(SimpleUDP)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(client)
add_subdirectory(server)

add_custom_target(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} server client)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_PATH})
