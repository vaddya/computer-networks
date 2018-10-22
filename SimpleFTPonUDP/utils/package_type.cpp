#include "package_type.h"

std::string type2string(PackageType type) {
    switch (type) {
        case PackageType::REQUEST:
            return "request";
        case PackageType::RESPONSE:
            return "response";
        case PackageType::ACK:
            return "ack";
        default:
            return "unknown";
    }
}
