cmake_minimum_required(VERSION 3.10)
project(utils)

set(utils_src readn.h readn.cpp)

add_library(utils STATIC ${utils_src} readn.h)