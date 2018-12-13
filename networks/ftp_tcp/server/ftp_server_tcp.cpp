#include "ftp_server_tcp.h"

FTPServerTCP::FTPServerTCP(int socket, pthread_t *thread) {
    socket_id = socket;
    thread_id = thread;
    path = std::string(SERVER_PATH);
    io = new SocketIO(socket_id);
}

FTPServerTCP::~FTPServerTCP() {
    disconnect();
}

void FTPServerTCP::processRequests() {
    while (true) {
        auto request = io->getRequest();
        std::cout << "Got request <" << request2string(request) << "> from socket " << socket_id << std::endl;
        switch (request) {
            case Request::PWD:
                pwd();
                break;
            case Request::LS:
                ls();
                break;
            case Request::CD:
                cd();
                break;
            case Request::GET:
                get();
                break;
            case Request::PUT:
                put();
                break;
            case Request::DISCONNECT:
                return;
            default:
                std::cerr << "Unknown request: " << request << std::endl;
                break;
        }
    }
}

void FTPServerTCP::connect() {

}

void FTPServerTCP::pwd() {
    io->sendResponse(Response::OK);
    io->sendString(path.c_str());
}

void FTPServerTCP::ls() {
    io->sendResponse(Response::OK);
    size_t size = static_cast<size_t>(std::distance(
            fs::directory_iterator(path), fs::directory_iterator())
    );
    io->sendDataSize(size);
    for (auto &p : fs::directory_iterator(path)) {
        io->sendBool(fs::is_directory(p.path()));
        std::string str = p.path().filename();
        io->sendString(str);
    }
}

void FTPServerTCP::cd() {
    fs::path attempt = path / io->getString();
    if (!fs::exists(attempt)) {
        io->sendResponse(Response::NOT_EXISTS);
    } else if (!fs::is_directory(attempt)) {
        io->sendResponse(Response::NOT_DIRECTORY);
    } else {
        io->sendResponse(Response::OK);
        path = fs::canonical(attempt);
    }
}

void FTPServerTCP::get() {
    fs::path attempt = path / io->getString();
    if (!fs::exists(attempt)) {
        io->sendResponse(Response::NOT_EXISTS);
    } else if (!fs::is_regular_file(attempt)) {
        io->sendResponse(Response::NOT_REGULAR_FILE);
    } else {
        io->sendResponse(Response::OK);
        attempt = fs::canonical(attempt);
        std::ifstream file(attempt);
        io->sendFile(file);
    }
}

void FTPServerTCP::put() {
    fs::path attempt = path / io->getString();
    if (fs::exists(attempt)) {
        io->sendResponse(Response::ALREADY_EXISTS);
    } else {
        io->sendResponse(Response::OK);
        std::ofstream file(attempt);
        io->getFile(file);
    }
}

void FTPServerTCP::disconnect() {
    shutdown(socket_id, SHUT_RDWR);
    close(socket_id);
    delete thread_id;
    delete io;
}

std::string FTPServerTCP::toString() {
    return std::string("[") + std::to_string(socket_id) + ", " + std::to_string(*thread_id) + "]";
}

