project(ftp_tcp)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(utils)

add_custom_target(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} ftp_tcp_server ftp_tcp_client)
