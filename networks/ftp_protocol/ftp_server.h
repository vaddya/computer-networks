#ifndef FTP_PROTOCOL_FTP_SERVER_H
#define FTP_PROTOCOL_FTP_SERVER_H

class FTPServer {
public:

    /**
     * Process connecting request
     */
    virtual void connect() = 0;

    /**
     * Process fetching current directory request
     */
    virtual void pwd() = 0;

    /**
     * Process fetching files in current directory request
     */
    virtual void ls() = 0;

    /**
     * Process changing directory request
     */
    virtual void cd() = 0;

    /**
     * Process file downloading request
     */
    virtual void get() = 0;

    /**
     * Process file uploading request
     */
    virtual void put() = 0;

    /**
     * Process disconnecting request
     */
    virtual void disconnect() = 0;

};

#endif //FTP_PROTOCOL_FTP_SERVER_H
