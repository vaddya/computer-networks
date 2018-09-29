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
#include "request.h"
#include "response.h"

const int PORT = 7000;

const int BUFFER_SIZE = 255;

class Client {

public:
    Client();

    ~Client();

    /**
     * Connects to server
     * @param server_addr
     * @param port
     */
    void conn(std::string server_addr, int port);

    /**
     * Disconnects from current server
     */
    void disconn();

    /**
     * List files and folders in current directory
     * @return list of files and folders
     */
    std::vector<std::string> ls();

    /**
     * Change current directory
     * @param path
     */
    void cd(std::string path);

    /**
     * Download file from server
     * @param file_name
     * @param dst
     */
    void get(std::string file_name, std::ofstream dst);

    /**
     * Upload file to server
     * @param file_name
     * @param src
     */
    void put(std::string file_name, std::ifstream src);

private:
    int *s;

    char *buffer;

    void sendCommand(Request request) {
        buffer[0] = request;
        send(*this->s, buffer, sizeof buffer[0], nullptr);
    }
};

#endif //SIMPLECLIENTSERVER_CLIENT_H
