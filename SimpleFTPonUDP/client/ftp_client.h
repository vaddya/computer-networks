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
     * Get current working directory
     * @return directory
     */
    std::string pwd();

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
    const std::string CLIENT_PATH = "/home/vaddya/FTP/Client/";

    int s;
    sockaddr_in peer;
    socklen_t peer_size;
    size_t counter;

    SocketIO *io;

    void validate();
};

#endif //SIMPLEFTPONUDP_CLIENT_H
