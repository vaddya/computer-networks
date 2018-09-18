#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

const int PORT = 7000;

const int BUFFER_SIZE = 10;

void readn(int s, char *buf, int n) {
    size_t m = 0;
    while (m < n) {
        m += recv(s, buf + m, n - m, 0);
    }
}

int main(void) {
    struct sockaddr_in local;
    int s;
    int s1;
    int rc;
    char buf[BUFFER_SIZE + 1];
    buf[BUFFER_SIZE] = '\0';

    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket call failed");
        exit(1);
    }
    rc = bind(s, (struct sockaddr *) &local, sizeof(local));
    if (rc < 0) {
        perror("bind call failure");
        exit(1);
    }
    rc = listen(s, 5);
    if (rc) {
        perror("listen call failed");
        exit(1);
    }
    s1 = accept(s, NULL, NULL);
    if (s1 < 0) {
        perror("accept call failed");
        exit(1);
    }

//    rc = recv(s1, buf, BUFFER_SIZE, 0);
    while (buf[0] != 'q') {
        readn(s1, buf, BUFFER_SIZE);
        printf("server recv: <%s>\n", buf);
        rc = send(s1, buf, BUFFER_SIZE, 0);
    }

//    if (rc <= 0) {
//        perror("recv call failed");
//        exit(1);
//    }

    shutdown(s1, SHUT_RDWR);
    close(s1);

    shutdown(rc, SHUT_RDWR);
    close(rc);

    exit(0);
}
