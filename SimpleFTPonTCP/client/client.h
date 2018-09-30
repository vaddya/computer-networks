#ifndef SIMPLECLIENTSERVER_CLIENT_H
#define SIMPLECLIENTSERVER_CLIENT_H

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
#include <socket_io.h>
#include "readn.h"
#include "request.h"
#include "response.h"

const int PORT = 7000;


class Client {

public:
    Client();

    ~Client();

    /**
     * Connects to server
     * @param server_addr
     * @param port
     */
    void connect(const std::string &server_addr, int port);

    /**
     * Disconnects from current server
     */
    void disconnect();

    /**
     * List files and folders in current directory
     * @return list of files and folders
     */
    std::vector<std::string> ls();

    /**
     * Change current directory
     * @param path
     */
    void cd(const std::string &string);

    /**
     * Download file from server
     * @param file_name
     * @param dst
     */
    void get(const std::string &file_name);

    /**
     * Upload file to server
     * @param file_name
     * @param src
     */
    void put(const std::string &file_name);

private:
    int *s;

    SocketIO *io;
};

#endif //SIMPLECLIENTSERVER_CLIENT_H
