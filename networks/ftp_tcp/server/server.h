#ifndef FTP_TCP_SERVER_H
#define FTP_TCP_SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "ftp_server_tcp.h"

const int PORT = 7000;

const char *SERVER_HELP = R"(
Supported commands:
 - list
 - kill x
 - exit
)";

/**
 * Global vector of clients
 */
std::vector<FTPServerTCP *> servers;

/**
 * Global mutex to safe access vector of servers
 */
pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

/**
 * Client thread
 * @param data - client socket
 */
void *clientThread(void *data);

/**
 * Thread to accept connections
 * @param data - server socket
 */
void *acceptThread(void *data);

#endif //FTP_TCP_SERVER_H
