#ifndef FTP_UDP_PACKAGE_TYPE_H
#define FTP_UDP_PACKAGE_TYPE_H

#include <string>

enum PackageType : unsigned char {
    REQUEST = 0,
    RESPONSE = 1,
    ACK = 2,
    EMPTY = 3
};

std::string type2string(PackageType type);

#endif //FTP_UDP_PACKAGE_TYPE_H
