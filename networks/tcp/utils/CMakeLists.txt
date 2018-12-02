project(tcp_utils)

set(TCP_UTILS_SRC readn.h readn.cpp)

add_library(${PROJECT_NAME} STATIC ${TCP_UTILS_SRC})