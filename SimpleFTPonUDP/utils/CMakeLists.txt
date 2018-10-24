cmake_minimum_required(VERSION 3.10)

project(utils)

set(UTILS_SRC request.h request.cpp response.h response.cpp socket_io.cpp socket_io.h package_type.h package_type.cpp package.h package.cpp peer.h)

add_library(${PROJECT_NAME} STATIC ${UTILS_SRC})