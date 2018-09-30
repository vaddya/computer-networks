#ifndef SIMPLEFTPONTCP_SOCKETREADER_H
#define SIMPLEFTPONTCP_SOCKETREADER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "request.h"
#include "response.h"
#include "readn.h"

const size_t BUFFER_SIZE = 255;

class SocketIO {

public:
    explicit SocketIO(int *socket);

    ~SocketIO();

    ssize_t sendRequest(Request request);

    ssize_t sendString(const std::string &string);

    ssize_t sendDataSize(size_t size);

    ssize_t sendData(const char *buffer, size_t size);

    void sendFile(std::ifstream &file);

    Response getResponse();

    std::string getString();

    size_t getDataSize();

    ssize_t getData(char *buffer, size_t max_size);

    void getFile(std::ofstream &file);

private:
    int *s;

    char *buffer;
};

#endif //SIMPLEFTPONTCP_SOCKETREADER_H
