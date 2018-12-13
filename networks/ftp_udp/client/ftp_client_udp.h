#ifndef SIMPLEFTPONUDP_CLIENT_H
#define SIMPLEFTPONUDP_CLIENT_H

#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include "request.h"
#include "response.h"
#include "socket_io.h"
#include "ftp_entity.h"
#include "ftp_client.h"

class FTPClientUDP : FTPClient {

public:
    FTPClientUDP();

    virtual ~FTPClientUDP();

    void connect(const std::string &server_addr, int port);

    void disconnect();

    std::string pwd();

    std::vector<FTPEntity> ls();

    void cd(const std::string &string);

    void get(const std::string &file_name);

    void put(const std::string &file_name);

private:
    const std::string CLIENT_PATH = "/home/vaddya/FTP/Client/";

    int s;
    sockaddr_in peer;
    size_t counter;

    SocketIO *io;

    void validate();
};

#endif //SIMPLEFTPONUDP_CLIENT_H
