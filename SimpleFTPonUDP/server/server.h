#ifndef SIMPLEFTPONUDP_FTP_SERVER_H
#define SIMPLEFTPONUDP_FTP_SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <map>
#include "ftp_server.h"
#include "peer.h"

const int PORT = 7000;

const char *SERVER_HELP = R"(
Supported commands:
 - list
 - kill x
 - exit
)";

pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

std::map<Peer, FTPServer *> servers;

void *proc_thread(void *data);

#endif //SIMPLEFTPONUDP_FTP_SERVER_H
