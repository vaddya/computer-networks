#include "client.h"

int main(int argc, char **argv) {
    sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(PORT);
    peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    char recv_buf[BUFFER_SIZE];
    memset(recv_buf, 0, BUFFER_SIZE);
    char const *send_buf;
    if (argc > 1) {
        send_buf = argv[1];
    } else {
        send_buf = "Hello, World!";
    }

    auto s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        std::cerr << "socket call failed" << std::endl;
        return 1;
    }

    sendto(s, send_buf, strlen(send_buf), 0, (sockaddr *) &peer, sizeof(peer));
    std::cout << "client send: " << send_buf << std::endl;

    sockaddr_in from{};
    socklen_t from_size = sizeof(from);
    recvfrom(s, recv_buf, BUFFER_SIZE, 0, (sockaddr *) &from, &from_size);
    std::cout << "client received back: " << recv_buf << std::endl;

    shutdown(s, SHUT_RDWR);
    close(s);

    return 0;
}
