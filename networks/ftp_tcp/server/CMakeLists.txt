project(ftp_tcp_server)

add_executable(${PROJECT_NAME}
        server.h server.cpp
        ftp_server_tcp.h ftp_server_tcp.cpp)

target_link_libraries(${PROJECT_NAME} pthread stdc++fs ftp_protocol ftp_tcp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../../ftp_protocol)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)
