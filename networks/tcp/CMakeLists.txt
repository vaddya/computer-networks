project(tcp)

add_subdirectory(server)
add_subdirectory(server_select)
add_subdirectory(client)
add_subdirectory(utils)

add_custom_target(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} tcp_server tcp_server_select tcp_client)
