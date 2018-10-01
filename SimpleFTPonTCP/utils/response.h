#ifndef SIMPLEFTPONTCP_RESPONSE_H
#define SIMPLEFTPONTCP_RESPONSE_H

enum Response : unsigned char {
    OK = 0,
    ERROR = 1,
    CD_NOT_DIRECTORY = 2,
    CD_NOT_EXIST = 3
};

#endif //SIMPLEFTPONTCP_RESPONSE_H
