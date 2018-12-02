#include "request.h"

std::string request2string(Request request) {
    switch (request) {
        case CONNECT:
            return "connect";
        case DISCONNECT:
            return "disconnect";
        case PWD:
            return "pwd";
        case LS:
            return "ls";
        case CD:
            return "cd";
        case GET:
            return "get";
        case PUT:
            return "put";
        default:
            return "unknown";
    }
}