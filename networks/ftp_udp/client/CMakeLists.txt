project(ftp_udp_client)

add_executable(${PROJECT_NAME}
        ftp_client.h ftp_client.cpp
        client.h client.cpp)

target_link_libraries(${PROJECT_NAME} pthread ftp_udp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)