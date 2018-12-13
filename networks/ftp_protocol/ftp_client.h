#ifndef NETWORKS_I_FTP_CLIENT_H
#define NETWORKS_I_FTP_CLIENT_H

#include "ftp_entity.h"

class FTPClient {
public:

    /**
     * Connects to server
     * @param server_addr
     * @param port
     */
    virtual void connect(const std::string &server_addr, int port) = 0;

    /**
     * Disconnects from current server
     */
    virtual void disconnect() = 0;

    /**
     * Get current working directory
     * @return directory
     */
    virtual std::string pwd() = 0;

    /**
     * List files and folders in current directory
     * @return list of files and folders
     */
    virtual std::vector<FTPEntity> ls() = 0;

    /**
     * Change current directory
     * @param path
     */
    virtual void cd(const std::string &string) = 0;

    /**
     * Download file from server
     * @param file_name
     * @param dst
     */
    virtual void get(const std::string &file_name) = 0;

    /**
     * Upload file to server
     * @param file_name
     * @param src
     */
    virtual void put(const std::string &file_name) = 0;

};

#endif //NETWORKS_I_FTP_CLIENT_H
