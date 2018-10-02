#ifndef SIMPLEFTPONTCP_FTP_SERVER_H
#define SIMPLEFTPONTCP_FTP_SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "ftp_server.h"

const int PORT = 7000;

/**
 * Global vector of clients
 */
std::vector<FTPServer *> servers;

/**
 * Global mutex to safe access vector of servers
 */
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/**
 * Client thread
 * @param data - client socket
 */
void *client_thread(void *data);

/**
 * Thread to accept connections
 * @param data - server socket
 */
void *accept_thread(void *data);

#endif //SIMPLEFTPONTCP_FTP_SERVER_H
