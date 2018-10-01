#include "server.h"

Server::Server(int *socket, pthread_t *thread) {
    socket_id = socket;
    thread_id = thread;
    path = std::string(SERVER_PATH);
    io = new SocketIO(socket_id);
}

Server::~Server() {
    delete socket_id;
    delete thread_id;
    delete io;
}

void Server::processRequests() {
    while (true) {
        auto request = io->getRequest();
        std::cout << "Got request " << request << " from socket " << *socket_id << std::endl;
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
            default:
                std::cout << "Unknown request: " << request << std::endl;
                return;
        }
    }
}

void Server::kill_and_join() {
    std::cout << "Killing client with socket: " << *socket_id << std::endl;
    shutdown(*socket_id, SHUT_RDWR);
    close(*socket_id);
    pthread_join(*thread_id, nullptr);
}

void Server::pwd() {
    io->sendResponse(Response::OK);
    io->sendString(path.c_str());
}

void Server::ls() {
    io->sendResponse(Response::OK);
    size_t size = static_cast<size_t>(std::distance(
            fs::directory_iterator(path), fs::directory_iterator())
    );
    io->sendDataSize(size);
    for (auto &p : fs::directory_iterator(path)) {
        io->sendBool(fs::is_directory(p.path()));
        std::string str = p.path().string();
        io->sendString(str);
    }
}

void Server::cd() {
    fs::path attempt = path / io->getString();
    if (!fs::exists(attempt)) {
        io->sendResponse(Response::CD_NOT_EXIST);
    } else if (!fs::is_directory(attempt)) {
        io->sendResponse(Response::CD_NOT_DIRECTORY);
    } else {
        path = fs::canonical(attempt);
        io->sendResponse(Response::OK);
    }
}

void Server::get() {

}

void Server::put() {

}

std::string Server::to_string() {
    return std::string("[") + std::to_string(*socket_id) + ", " + std::to_string(*thread_id) + "]";
}