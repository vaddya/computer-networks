#ifndef SIMPLEFTPONUDP_CLIENT_H
#define SIMPLEFTPONUDP_CLIENT_H

#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include "request.h"
#include "response.h"
#include "socket_io.h"

class FTPClient {

public:
    FTPClient();

    ~FTPClient();

    void connect(const std::string &server_addr, int port);

    void disconnect();

    std::string pwd();

    std::vector<std::string> ls();

    void cd(const std::string &string);

    void get(const std::string &file_name);

    void put(const std::string &file_name);

private:
    const std::string CLIENT_PATH = "/home/vaddya/FTP/Client/";

    int s;
    sockaddr_in peer;
    socklen_t peer_size;
    size_t counter;

    SocketIO *io;

    void validate();
};

#endif //SIMPLEFTPONUDP_CLIENT_H
