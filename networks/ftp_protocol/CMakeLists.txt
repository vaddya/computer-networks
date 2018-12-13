project(ftp_protocol)

set(FTP_UTILS_SRC
        ftp_server.h ftp_client.h
        ftp_entity.h ftp_peer.h
        request.h request.cpp
        response.h response.cpp)

add_library(${PROJECT_NAME} STATIC ${FTP_UTILS_SRC})