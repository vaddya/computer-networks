#ifndef SIMPLEFTPONTCP_SERVER_H
#define SIMPLEFTPONTCP_SERVER_H

#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include "response.h"
#include "socket_io.h"

const std::string SERVER_PATH = "/home/vaddya/Test";

namespace fs = std::experimental::filesystem;

class Server {

public:
    Server(int *socket, pthread_t *thread);

    ~Server();

    void processRequests();

    void kill_and_join();

    void pwd();

    void ls();

    void cd();

    void get();

    void put();

    std::string to_string();

private:
    fs::path path;

    int *socket_id;

    pthread_t *thread_id;

    SocketIO *io;
};


#endif //SIMPLEFTPONTCP_SERVER_H
