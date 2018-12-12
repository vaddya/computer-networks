project(ftp_tcp_server)

add_executable(${PROJECT_NAME}
        server.h server.cpp
        ftp_server.h ftp_server.cpp)

target_link_libraries(${PROJECT_NAME} pthread stdc++fs ftp_utils ftp_tcp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../../ftp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)
