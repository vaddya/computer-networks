cmake_minimum_required(VERSION 3.10)
project(utils)

set(utils_src readn.h readn.cpp request.h response.h socket_io.cpp socket_io.h)

add_library(utils STATIC ${utils_src} readn.h request.h response.h socket_io.h)