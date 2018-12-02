project(udp)

add_subdirectory(client)
add_subdirectory(server)

add_custom_target(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} udp_server udp_client)
