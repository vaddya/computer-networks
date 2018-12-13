project(ftp_tcp_utils)

set(FTP_TCP_UTILS_SRC
        readn.h readn.cpp
        socket_io.h socket_io.cpp)

add_library(${PROJECT_NAME} STATIC ${FTP_TCP_UTILS_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ../../ftp_protocol)

target_link_libraries(${PROJECT_NAME} ftp_protocol)
