#ifndef SIMPLEFTPONUDP_REQUEST_H
#define SIMPLEFTPONUDP_REQUEST_H

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

#endif //SIMPLEFTPONUDP_REQUEST_H
