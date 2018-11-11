#ifndef SIMPLEFTPONUDP_FTPSERVER_H
#define SIMPLEFTPONUDP_FTPSERVER_H

#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include "request.h"
#include "response.h"
#include "socket_io.h"

namespace fs = std::experimental::filesystem;

class FTPServer {

public:
    FTPServer(SocketIO *io, sockaddr_in peer, socklen_t peer_size);

    void process_request(const Package &req);

private:
    void connect(const Package &req);

    void pwd(const Package &req);

    void ls(const Package &req);

    void cd(const Package &req);

    void get(const Package &req);

    void put(const Package &req);

    const std::string SERVER_PATH = "/home/vaddya/FTP/Server/";

    size_t counter;
    sockaddr_in peer;
    socklen_t peer_size;
    SocketIO *io;
    fs::path path;
};

#endif //SIMPLEFTPONUDP_FTPSERVER_H