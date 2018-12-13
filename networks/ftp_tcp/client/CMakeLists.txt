project(ftp_tcp_client)

add_executable(${PROJECT_NAME}
        client.h client.cpp
        ftp_client_tcp.h ftp_client_tcp.cpp)

target_link_libraries(${PROJECT_NAME} pthread stdc++fs ftp_protocol ftp_tcp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../../ftp_protocol)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)
