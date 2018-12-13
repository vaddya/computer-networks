project(ftp_udp_client)

add_executable(${PROJECT_NAME}
        ftp_client_udp.h ftp_client_udp.cpp
        client.h client.cpp)

target_link_libraries(${PROJECT_NAME} pthread ftp_protocol ftp_udp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../../ftp_protocol)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)