cmake_minimum_required(VERSION 3.10)

project(networks)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(tcp)
add_subdirectory(udp)
add_subdirectory(ftp_tcp)
add_subdirectory(ftp_udp)
add_subdirectory(ftp_utils)
