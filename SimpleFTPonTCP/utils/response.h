#ifndef SIMPLEFTPONTCP_RESPONSE_H
#define SIMPLEFTPONTCP_RESPONSE_H

#include <string>

enum Response : unsigned char {
    OK = 0,
    ERROR = 1,
    NOT_DIRECTORY = 2,
    NOT_REGULAR_FILE = 3,
    NOT_EXISTS = 4,
    ALREADY_EXISTS = 5
};

std::string response2string(Response response);

#endif //SIMPLEFTPONTCP_RESPONSE_H
