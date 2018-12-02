project(tcp_server_select)

add_executable(${PROJECT_NAME} server_select.h server_select.cpp)

target_link_libraries(${PROJECT_NAME} pthread)