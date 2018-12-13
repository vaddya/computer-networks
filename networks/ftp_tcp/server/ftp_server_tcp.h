#ifndef SIMPLEFTPONTCP_SERVER_H
#define SIMPLEFTPONTCP_SERVER_H

#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include "response.h"
#include "socket_io.h"
#include "ftp_server.h"

namespace fs = std::experimental::filesystem;

class FTPServerTCP : FTPServer {

public:
    FTPServerTCP(int socket, pthread_t *thread);

    virtual ~FTPServerTCP();

    void processRequests();

    void connect() override;

    void pwd() override;

    void ls() override;

    void cd() override;

    void get() override;

    void put() override;

    void disconnect() override;

    std::string toString();

private:
    const std::string SERVER_PATH = "/home/vaddya/FTP/Server/";

    fs::path path;

    int socket_id;

    pthread_t *thread_id;

    SocketIO *io;
};


#endif //SIMPLEFTPONTCP_SERVER_H
