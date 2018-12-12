project(ftp_udp_utils)

set(FTP_UDP_UTILS_SRC
        socket_io.h socket_io.cpp
        package_type.h package_type.cpp
        package.h package.cpp
        peer)

add_library(${PROJECT_NAME} STATIC ${FTP_UDP_UTILS_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ../../ftp_utils)
