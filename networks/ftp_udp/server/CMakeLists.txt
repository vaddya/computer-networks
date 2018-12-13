project(ftp_udp_server)

add_executable(${PROJECT_NAME}
        ftp_server_udp.h ftp_server_udp.cpp
        server.h server.cpp)

target_link_libraries(${PROJECT_NAME} pthread stdc++fs ftp_protocol ftp_udp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../../ftp_protocol)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)
