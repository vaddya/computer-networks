project(ftp_utils)

set(FTP_UTILS_SRC
        request.h request.cpp
        response.h response.cpp)

add_library(${PROJECT_NAME} STATIC ${FTP_UTILS_SRC})