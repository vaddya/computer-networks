#include "client.h"

int main(int argc, char **argv) {
    struct sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(PORT);
    peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // inet_addr("127.0.0.1");

    char recvBuf[BUFFER_SIZE];
    char const *sendBuf;
    if (argc > 1) {
        sendBuf = argv[1];
    } else {
        sendBuf = "Hello, World!";
    }

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket call failed");
        return 1;
    }

    int rc = connect(s, (struct sockaddr *) &peer, sizeof(peer));
    if (rc) {
        perror("connect call failed");
        return 1;
    }

    rc = send(s, sendBuf, strlen(sendBuf), 0);
    if (rc <= 0) {
        perror("send call failed");
        return 1;
    }

    rc = recv(s, recvBuf, BUFFER_SIZE, 0);
    if (rc <= 0)
        perror("recv call failed");
    else
        printf("client recv: %s\n", recvBuf);

    shutdown(rc, SHUT_RDWR);
    close(rc);

    return 0;
}
