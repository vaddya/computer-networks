#ifndef SIMPLEFTPONUDP_RESPONSE_H
#define SIMPLEFTPONUDP_RESPONSE_H

#include <string>

enum Response : unsigned char {
    OK = 0,
    ERROR = 1,
    FILE_DATA = 2,
    FILE_DATA_END = 3,
    NOT_DIRECTORY = 4,
    NOT_REGULAR_FILE = 5,
    NOT_EXISTS = 6,
    ALREADY_EXISTS = 7,
};

std::string response2string(Response response);

#endif //SIMPLEFTPONUDP_RESPONSE_H
