#include "response.h"

std::string response2string(Response response) {
    switch (response) {
        case Response::OK:
            return "ok";
        case Response::ERROR:
            return "error";
        case Response::FILE_DATA:
            return "file";
        case Response::FILE_DATA_END:
            return "eof";
        case Response::NOT_DIRECTORY:
            return "not directory";
        case Response::NOT_REGULAR_FILE:
            return "not regular file";
        case Response::NOT_EXISTS:
            return "not exists";
        case Response::ALREADY_EXISTS:
            return "already exists";
        default:
            return "unknown";
    }
}
