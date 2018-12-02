project(tcp_server)

add_executable(${PROJECT_NAME} server.h server.cpp)

target_link_libraries(${PROJECT_NAME} pthread tcp_utils)

target_include_directories(${PROJECT_NAME} PUBLIC ../utils)