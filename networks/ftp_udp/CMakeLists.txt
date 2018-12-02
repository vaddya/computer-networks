project(ftp_udp)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(utils)

add_custom_target(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} ftp_udp_server ftp_udp_client)
