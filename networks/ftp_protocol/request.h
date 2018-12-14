#ifndef FTP_PROTOCOL_REQUEST_H
#define FTP_PROTOCOL_REQUEST_H

#include <string>

enum Request : unsigned char {
    CONNECT = 0,
    DISCONNECT = 1,
    PWD = 2,
    LS = 3,
    CD = 4,
    GET = 5,
    PUT = 6
};

std::string request2string(Request request);

#endif //FTP_PROTOCOL_REQUEST_H
