#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

const int PORT = 7000;

const int BUFFER_SIZE = 65507;

#endif //UDP_CLIENT_H
