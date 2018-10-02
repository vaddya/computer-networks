#include "client.h"

Client::Client() {
    s = nullptr;
}

Client::~Client() {
    disconnect();
}

void Client::connect(const std::string &server_addr, int port) {
    disconnect();
    sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(static_cast<uint16_t>(port));
    if (server_addr != "localhost") {
        peer.sin_addr.s_addr = inet_addr(server_addr.c_str());
    } else {
        peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    s = new int;
    *s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        std::cerr << "socket call failed" << std::endl;
    }

    if (::connect(*s, (struct sockaddr *) &peer, sizeof(peer))) {
        std::cerr << "connecting to " << server_addr << ":" << port << " failed" << std::endl;
    }

    io = new SocketIO(s);
}

void Client::disconnect() {
    if (s) {
        io->sendRequest(Request::DISCONNECT);
        shutdown(*s, SHUT_RDWR);
        close(*s);
        delete io;
        io = nullptr;
        delete s;
        s = nullptr;
    }
}

std::string Client::pwd() {
    io->sendRequest(Request::PWD);
    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "pwd failed: " << response << std::endl;
        return "";
    }
    return io->getString();
}

Client::FileList Client::ls() {
    io->sendRequest(Request::LS);
    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "ls failed: " << response << std::endl;
        return FileList();
    }
    size_t size = io->getDataSize();
    FileList names;
    for (auto i = 0; i < size; i++) {
        bool is_dir = io->getBool();
        std::string name = io->getString();
        names.emplace_back(is_dir, name);
    }
    return names;
}

void Client::cd(const std::string &path) {
    io->sendRequest(Request::CD);
    io->sendString(path);

    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "cd failed: " << response << std::endl;
        return;
    }
}

void Client::get(const std::string &file_name) {
    io->sendRequest(Request::GET);
    io->sendString(file_name);

    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "get failed: " << response << std::endl;
        return;
    }

    std::ofstream file("/home/vaddya/Test/Got/" + file_name);
    io->getFile(file);
}

void Client::put(const std::string &file_name) {
    io->sendRequest(Request::PUT);
    io->sendString(file_name);

    std::ifstream file(file_name);
    io->sendFile(file);

    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "put failed: " << response << std::endl;
        return;
    }
}
