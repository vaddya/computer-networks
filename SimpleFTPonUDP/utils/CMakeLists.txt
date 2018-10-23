cmake_minimum_required(VERSION 3.10)

project(utils)

set(UTILS_SRC readn.h readn.cpp request.h response.h socket_io.cpp socket_io.h request.cpp response.cpp package_type.h package_type.cpp package.cpp package.h peer.h)

add_library(${PROJECT_NAME} STATIC ${UTILS_SRC})