project(ftp_tcp_client)

add_executable(${PROJECT_NAME}
        ftp_client.h ftp_client.cpp
        client.h client.cpp
        ftp_entity.h)

target_link_libraries(${PROJECT_NAME} pthread stdc++fs ftp_tcp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)
