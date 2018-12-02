#include "server_select.h"

int main(int argc, char **argv) {
    struct sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    char buf[BUFFER_SIZE + 1];
    buf[BUFFER_SIZE] = '\0';

    int ss = socket(AF_INET, SOCK_STREAM, 0);
    if (ss < 0) {
        perror("socket call failed");
        return 1;
    }
    int enable = 1;
    setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    int rc = bind(ss, (struct sockaddr *) &local, sizeof(local));
    if (rc < 0) {
        perror("bind call failure");
        return 1;
    }
    rc = listen(ss, 5);
    if (rc) {
        perror("listen call failed");
        return 1;
    }

    std::vector<int> clients;

    fd_set set;
    int maxRequests = 10;
    while (maxRequests--) {
        FD_ZERO(&set);
        FD_SET(ss, &set);

        int max_id = ss;
        for (auto cs : clients) {
            FD_SET(cs, &set);
            if (cs > max_id) {
                max_id = cs;
            }
        }

        select(max_id + 1, &set, nullptr, nullptr, nullptr);

        for (auto cs : clients) {
            if (FD_ISSET(cs, &set)) {
                rc = recv(cs, buf, BUFFER_SIZE, MSG_NOSIGNAL);
                buf[rc] = '\0';
                std::cout << "Receive " << rc << " bytes from socket " << cs << ": " << buf << std::endl;
                send(cs, buf, BUFFER_SIZE, 0);
            }
        }
        if (FD_ISSET(ss, &set)) {
            int cs = accept(ss, nullptr, nullptr);
            if (cs < 0) {
                perror("accept call failed");
                return 1;
            }
            std::cout << "Accepted client with socket " << cs << std::endl;
            clients.push_back(cs);
        }
    }

    for (auto cs : clients) {
        shutdown(cs, SHUT_RDWR);
        close(cs);
    }

    shutdown(rc, SHUT_RDWR);
    close(rc);

    return 0;
}