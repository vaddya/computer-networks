#include "server.h"

int main(int argc, char **argv) {
    sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    auto s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        std::cerr << "socket call failed" << std::endl;
        return 1;
    }
    int enable = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    int rc = bind(s, (sockaddr *) &local, sizeof(local));
    if (rc < 0) {
        std::cerr << "bind call failure" << std::endl;
        return 1;
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    sockaddr_in from{};
    socklen_t from_size = sizeof(from);
    ssize_t a = recvfrom(s, buffer, BUFFER_SIZE, 0, (sockaddr *) &from, &from_size);
    std::cout << "server received: " << buffer << std::endl;

    buffer[0] = 'X';
    sendto(s, buffer, BUFFER_SIZE, 0, (sockaddr *) &from, from_size);
    std::cout << "server send back: " << buffer << std::endl;

    shutdown(s, SHUT_RDWR);
    close(s);
    return 0;
}