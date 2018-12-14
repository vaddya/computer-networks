#ifndef FTP_TCP_SOCKET_IO_H
#define FTP_TCP_SOCKET_IO_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "readn.h"
#include "request.h"
#include "response.h"

class SocketIO {

public:
    explicit SocketIO(int socket);

    ~SocketIO();

    ssize_t sendRequest(Request request);

    ssize_t sendResponse(Response response);

    ssize_t sendBool(bool b);

    ssize_t sendString(const std::string &string);

    ssize_t sendDataSize(size_t size);

    ssize_t sendData(const char *buffer, size_t size);

    void sendFile(std::ifstream &file);

    Response getResponse();

    Request getRequest();

    bool getBool();

    std::string getString();

    size_t getDataSize();

    ssize_t getData(char *buffer, size_t max_size);

    void getFile(std::ofstream &file);

private:
    const size_t BUFFER_SIZE = 255;

    int s;

    char *buffer;
};

#endif //FTP_TCP_SOCKET_IO_H
