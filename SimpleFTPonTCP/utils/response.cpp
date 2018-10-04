#include "response.h"

std::string response2string(Response response) {
    switch (response) {
        case OK:
            return "ok";
        case ERROR:
            return "error";
        case NOT_DIRECTORY:
            return "not directory";
        case NOT_REGULAR_FILE:
            return "not regular file";
        case NOT_EXISTS:
            return "not exists";
        case ALREADY_EXISTS:
            return "already exists";
        default:
            return "unknown";
    }
}
