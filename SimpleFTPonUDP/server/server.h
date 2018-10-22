#ifndef SIMPLEFTPONUDP_FTP_SERVER_H
#define SIMPLEFTPONUDP_FTP_SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <map>
#include "ftp_server.h"

const int PORT = 7000;

class Peer {

public:
    explicit Peer(const sockaddr_in &peer) {
        ip = inet_ntoa(peer.sin_addr);
        port = peer.sin_port;
    }

    bool operator<(const Peer &rhs) const {
        return (ip < rhs.ip) && (port < rhs.port);
    }

    std::string toString() const {
        return ip + ":" + std::to_string(port);
    }

private:
    std::string ip;
    int port;
};

const char *SERVER_HELP = R"(
Supported commands:
 - list
 - kill x
 - exit
)";

bool end = false;

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

std::map<Peer, FTPServer*> servers;

void *proc_thread(void *data);

#endif //SIMPLEFTPONUDP_FTP_SERVER_H
