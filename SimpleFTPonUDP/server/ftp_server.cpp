#include "ftp_server.h"

FTPServer::FTPServer(SocketIO *io, sockaddr_in peer, socklen_t peer_size) : io(io), peer(peer), peer_size(peer_size) {
    path = std::string(SERVER_PATH);
    counter = 0;
}

void FTPServer::process_request(const Package &req) {
    if (req.getCounter() != counter) {
        std::cerr << "Wrong package indexes: " << req.getCounter() << " instead of " << counter
                  << ", updating counter" << std::endl;
        counter = req.getCounter();
    }
    counter += 1; // usr msg
    io->sendTo(peer, Package::ack(counter, req.getCounter()));
    counter += 1; // our ack

    switch (req.getRequest()) {
        case Request::CONNECT:
            connect(req);
            break;
        case Request::PWD:
            pwd(req);
            break;
        case Request::LS:
            ls(req);
            break;
        case Request::CD:
            cd(req);
            break;
        case Request::GET:
            get(req);
            break;
        case Request::PUT:
            put(req);
            break;
        case Request::DISCONNECT:
            break;
        default:
            std::cerr << "Unknown request: " << req.getRequest() << std::endl;
            break;
    }
}

void FTPServer::connect(const Package &req) {
    io->sendTo(peer, Package::response(counter, Response::OK));
    counter += 2; // our msg + usr ack
}

void FTPServer::pwd(const Package &req) {
    io->sendTo(peer, Package::response(counter, Response::OK, path.c_str(), path.string().size()));
    counter += 2; // our msg + usr ack
}

void FTPServer::ls(const Package &req) {
    std::vector<std::string> list;
    for (auto &p : fs::directory_iterator(path)) {
        list.push_back(p.path().filename());
    }
    io->sendTo(peer, Package::response(counter, Response::OK, list));
    counter += 2; // our msg + usr ack
}

void FTPServer::cd(const Package &req) {
    fs::path attempt = path / req.extractString();
    if (!fs::exists(attempt)) {
        io->sendTo(peer, Package::response(counter, Response::NOT_EXISTS));
        counter += 2; // our msg + usr ack
    } else if (!fs::is_directory(attempt)) {
        io->sendTo(peer, Package::response(counter, Response::NOT_DIRECTORY));
        counter += 2; // our msg + usr ack
    } else {
        io->sendTo(peer, Package::response(counter, Response::OK));
        counter += 2; // our msg + usr ack
        path = fs::canonical(attempt);
    }
}

void FTPServer::get(const Package &req) {
    fs::path attempt = path / req.extractString();
    if (!fs::exists(attempt)) {
        io->sendTo(peer, Package::response(counter, Response::NOT_EXISTS));
        counter += 2; // our msg + usr ack
    } else if (!fs::is_regular_file(attempt)) {
        io->sendTo(peer, Package::response(counter, Response::NOT_REGULAR_FILE));
        counter += 2; // our msg + usr ack
    } else {
        io->sendTo(peer, Package::response(counter, Response::OK));
        counter += 2; // our msg + usr ack
        attempt = fs::canonical(attempt);
        std::ifstream file(attempt, std::ifstream::binary);
        counter = io->sendFile(peer, counter, file);
        counter += 2;
    }
}

void FTPServer::put(const Package &req) {
    fs::path attempt = path / req.extractString();
    if (fs::exists(attempt)) {
        io->sendTo(peer, Package::response(counter, Response::ALREADY_EXISTS));
        counter += 2; // our msg + usr ack
    } else {
        io->sendTo(peer, Package::response(counter, Response::OK));
        counter += 2; // our msg + usr ack
        std::ofstream file(attempt, std::ifstream::binary);
        counter = io->receiveFile(peer, counter, file);
        counter += 2;
    }
}
