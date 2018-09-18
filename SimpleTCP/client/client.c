#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int PORT = 7001;

const int BUFFER_SIZE = 255;

int main(int argc, char **argv) {
    struct sockaddr_in peer;
    int s;
    int rc;
    char recvBuf[BUFFER_SIZE];
    char *sendBuf;

    peer.sin_family = AF_INET;
    peer.sin_port = htons(PORT);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (argc > 1) {
        sendBuf = argv[1];
    } else {
        sendBuf = "Hello, World!";
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket call failed");
        exit(1);
    }

    rc = connect(s, (struct sockaddr *) &peer, sizeof(peer));
    if (rc) {
        perror("connect call failed");
        exit(1);
    }

    rc = send(s, sendBuf, strlen(sendBuf), 0);
    if (rc <= 0) {
        perror("send call failed");
        exit(1);
    }

    rc = recv(s, recvBuf, BUFFER_SIZE, 0);
    if (rc <= 0)
        perror("recv call failed");
    else
        printf("client recv: %s\n", recvBuf);

    shutdown(rc, SHUT_RDWR);
    close(rc);

    exit(0);
}
