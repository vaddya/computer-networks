#ifndef FTP_UDP_FPT_SERVER_UDP_H
#define FTP_UDP_FPT_SERVER_UDP_H

#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include "request.h"
#include "response.h"
#include "socket_io.h"
#include "ftp_server.h"

namespace fs = std::experimental::filesystem;

class FTPServerUDP : FTPServer {

public:
    FTPServerUDP(SocketIO *io, sockaddr_in peer, socklen_t peer_size);

    virtual ~FTPServerUDP();

    void processRequest(const Package &req);

    void connect() override;

    void pwd() override;

    void ls() override;

    void cd() override;

    void get() override;

    void put() override;

    void disconnect() override;

private:
    const std::string SERVER_PATH = "/home/vaddya/FTP/Server/";

    size_t counter;
    sockaddr_in peer;
    socklen_t peer_size;
    SocketIO *io;
    fs::path path;
    const Package *req;
};

#endif //FTP_UDP_FPT_SERVER_UDP_H
