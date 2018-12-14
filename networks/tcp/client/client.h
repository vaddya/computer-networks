#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

const int PORT = 7000;

const int BUFFER_SIZE = 255;

#endif //TCP_CLIENT_H
