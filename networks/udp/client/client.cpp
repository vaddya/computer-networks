#include "client.h"

int main(int argc, char **argv) {
    sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(PORT);
    peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    char recv_buf[BUFFER_SIZE];
    memset(recv_buf, 0, BUFFER_SIZE);

    char send_buf[BUFFER_SIZE + 1];
    memset(send_buf, 5, BUFFER_SIZE);
    send_buf[BUFFER_SIZE] = '\0';

    auto s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        std::cerr << "socket call failed" << std::endl;
        return 1;
    }

    auto ns = sendto(s, send_buf, 5, 0, (sockaddr *) &peer, sizeof(peer));
    std::cout << ns << "/" << strlen(send_buf) << std::endl;
    std::cout << "client send: " << send_buf << std::endl;

    sockaddr_in from{};
    socklen_t from_size;
    recvfrom(s, recv_buf, BUFFER_SIZE, 0, (sockaddr *) &from, &from_size);
    std::cout << "client received back: " << recv_buf << std::endl;

    shutdown(s, SHUT_RDWR);
    close(s);

    return 0;
}
