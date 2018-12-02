#include "readn.h"

int readn(int s, char *buf, int n) {
    int m = 0;
    ssize_t rc;
    while (m < n) {
        rc = recv(s, buf + m, n - m, MSG_NOSIGNAL);
        if (rc < 0) {
            return -1;
        }
        m += rc;
    }
    return m;
}