#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

const int PORT = 7000;

const int BUFFER_SIZE = 65507;

#endif //UDP_SERVER_H
