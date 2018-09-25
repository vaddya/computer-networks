#ifndef SIMPLECLIENTSERVER_SERVER_H
#define SIMPLECLIENTSERVER_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include "readn.h"

const int PORT = 7000;

const int BUFFER_SIZE = 10;

struct Client {
    int socket;
    pthread_t *thread_id;
};

/**
 * Global vector of clients
 */
std::vector<Client> clients;

/**
 * Global mutex to safe access vector of clients
 */
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Shut down client's socket & wait for client's thread termination
 * @param c - client
 */
void kill_and_join_client(Client &c);

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

#endif //SIMPLECLIENTSERVER_SERVER_H
