#ifndef SIMPLEFTPONTCP_CLIENT_H
#define SIMPLEFTPONTCP_CLIENT_H

#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include "request.h"
#include "response.h"
#include "socket_io.h"

class Client {

public:
    using FileList = std::vector<std::pair<bool, std::string>>;

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
     * Get current working directory
     * @return directory
     */
    std::string pwd();

    /**
     * List files and folders in current directory
     * @return list of files and folders
     */
    FileList ls();

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

#endif //SIMPLEFTPONTCP_CLIENT_H
