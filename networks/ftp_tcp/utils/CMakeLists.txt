project(ftp_tcp_utils)

set(FTP_TCP_UTILS_SRC
        readn.h readn.cpp
        request.h request.cpp
        response.h response.cpp
        socket_io.h socket_io.cpp)

add_library(${PROJECT_NAME} STATIC ${FTP_TCP_UTILS_SRC})
