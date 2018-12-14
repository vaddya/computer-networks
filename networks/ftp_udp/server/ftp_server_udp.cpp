#include "ftp_server_udp.h"

FTPServerUDP::FTPServerUDP(SocketIO *io, sockaddr_in peer, socklen_t peer_size) : io(io), peer(peer), peer_size(peer_size) {
    path = std::string(SERVER_PATH);
    counter = 0;
}

FTPServerUDP::~FTPServerUDP() {
    disconnect();
}

void FTPServerUDP::processRequest(const Package &req) {
    if (req.getCounter() == 4) { // oops
        return;
    }
    if (req.getCounter() != counter) {
        std::cerr << "Wrong package indexes: " << req.getCounter() << " instead of " << counter << std::endl;
        io->sendTo(peer, Package::response(counter, Response::ERROR));
        counter += 1;
        return;
    }
    counter += 1; // usr msg
    io->sendTo(peer, Package::ack(counter, req.getCounter()));
    counter += 1; // our ack
    this->req = &req;
    switch (req.getRequest()) {
        case Request::CONNECT:
            connect();
            break;
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
            disconnect();
            break;
        default:
            std::cerr << "Unknown request: " << req.getRequest() << std::endl;
            break;
    }
}

void FTPServerUDP::connect() {
    io->sendTo(peer, Package::response(counter, Response::OK));
    counter += 2; // our msg + usr ack
}

void FTPServerUDP::pwd() {
    io->sendTo(peer, Package::response(counter, Response::OK, path.c_str(), path.string().size()));
    counter += 2; // our msg + usr ack
}

void FTPServerUDP::ls() {
    std::vector<std::string> list;
    for (auto &p : fs::directory_iterator(path)) {
        list.push_back(p.path().filename());
    }
    io->sendTo(peer, Package::response(counter, Response::OK, list));
    counter += 2; // our msg + usr ack
}

void FTPServerUDP::cd() {
    fs::path attempt = path / req->extractString();
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

void FTPServerUDP::get() {
    fs::path attempt = path / req->extractString();
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

void FTPServerUDP::put() {
    fs::path attempt = path / req->extractString();
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

void FTPServerUDP::disconnect() {

}
