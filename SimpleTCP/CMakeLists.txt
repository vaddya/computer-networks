cmake_minimum_required(VERSION 3.10)

project(SimpleClientServer)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(client)

add_subdirectory(server)

add_custom_target(SimpleClientServer)

add_dependencies(SimpleClientServer server client)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_PATH})