#ifndef SIMPLEFTPONUDP_SOCKETREADER_H
#define SIMPLEFTPONUDP_SOCKETREADER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <cassert>
#include <iostream>
#include <fstream>
#include "request.h"
#include "response.h"
#include "package_type.h"
#include "package.h"
#include "ftp_peer.h"

class SocketIO {

public:
    explicit SocketIO(int socket);

    ~SocketIO();

    ssize_t sendTo(sockaddr_in peer, Package req);

    size_t sendFile(sockaddr_in peer, size_t counter, std::ifstream &file);

    Package receive(sockaddr *from, socklen_t *from_size);

    Package receiveFrom(sockaddr_in peer);

    size_t receiveFile(sockaddr_in peer, size_t counter, std::ofstream &file);

private:
    const size_t UDP_MAX_SIZE = 65507;
    const size_t FILE_BUFFER_SIZE = UDP_MAX_SIZE - sizeof(size_t) - sizeof(PackageType) - sizeof(Response);

    int s;
    char *out_buffer;
    char *in_buffer;
    char *file_buffer;
};


#endif //SIMPLEFTPONUDP_SOCKETREADER_H
