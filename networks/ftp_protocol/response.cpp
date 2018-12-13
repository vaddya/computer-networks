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
        case FILE_DATA:
            return "file";
        case FILE_DATA_END:
            return "eof";
        default:
            return "unknown";
    }
}
