#ifndef FTP_PROTOCOL_RESPONSE_H
#define FTP_PROTOCOL_RESPONSE_H

#include <string>

enum Response : unsigned char {
    OK = 0,
    ERROR = 1,
    NOT_DIRECTORY = 2,
    NOT_REGULAR_FILE = 3,
    NOT_EXISTS = 4,
    ALREADY_EXISTS = 5,
    FILE_DATA = 6,
    FILE_DATA_END = 7,
};

std::string response2string(Response response);

#endif //FTP_PROTOCOL_RESPONSE_H
