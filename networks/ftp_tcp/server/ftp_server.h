#ifndef SIMPLEFTPONTCP_SERVER_H
#define SIMPLEFTPONTCP_SERVER_H

#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include "response.h"
#include "socket_io.h"

namespace fs = std::experimental::filesystem;

class FTPServer {

public:
    FTPServer(int socket, pthread_t *thread);

    ~FTPServer();

    void process_requests();

    void close_socket();

    void join();

    void pwd();

    void ls();

    void cd();

    void get();

    void put();

    std::string to_string();

private:
    const std::string SERVER_PATH = "/home/vaddya/FTP/Server/";

    fs::path path;

    int socket_id;

    pthread_t *thread_id;

    SocketIO *io;
};


#endif //SIMPLEFTPONTCP_SERVER_H
