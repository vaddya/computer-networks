project(ftp_udp_server)

add_executable(${PROJECT_NAME}
        ftp_server.h ftp_server.cpp
        server.h server.cpp)

target_link_libraries(${PROJECT_NAME} pthread stdc++fs ftp_udp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)