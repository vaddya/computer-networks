#ifndef SIMPLEFTPONUDP_SOCKETREADER_H
#define SIMPLEFTPONUDP_SOCKETREADER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include "readn.h"
#include "request.h"
#include "response.h"
#include "package_type.h"
#include "package.h"

class SocketIO {

public:
    SocketIO(int socket);

    ~SocketIO();

    ssize_t send(sockaddr_in peer, Package req);

    void sendFile(sockaddr_in peer, size_t counter, std::ifstream &file);

    Package receive(sockaddr *from, socklen_t *from_size);

    Package receive();

    void receiveFile(std::ofstream &file);

private:
    const size_t UDP_MAX_SIZE = 65507;
    const size_t FILE_BUFFER_SIZE = UDP_MAX_SIZE - sizeof(int) - sizeof(PackageType);

    int s;

    char *out_buffer;
    char *in_buffer;
    char *file_buffer;
};


#endif //SIMPLEFTPONUDP_SOCKETREADER_H
