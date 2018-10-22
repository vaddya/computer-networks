#ifndef SIMPLEFTPONUDP_PACKAGETYPE_H
#define SIMPLEFTPONUDP_PACKAGETYPE_H

#include <string>

enum PackageType : unsigned char {
    REQUEST = 0,
    RESPONSE = 1,
    ACK = 2
};

std::string type2string(PackageType type);

#endif //SIMPLEFTPONUDP_PACKAGETYPE_H
