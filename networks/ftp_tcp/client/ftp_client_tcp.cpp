#include "ftp_client_tcp.h"

FTPClientTCP::FTPClientTCP() {
    s = 0;
}

FTPClientTCP::~FTPClientTCP() {
    disconnect();
}

void FTPClientTCP::connect(const std::string &server_addr, int port) {
    disconnect();
    sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(static_cast<uint16_t>(port));
    if (server_addr != "localhost") {
        peer.sin_addr.s_addr = inet_addr(server_addr.c_str());
    } else {
        peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        std::cerr << "socket call failed" << std::endl;
        return;
    }

    if (::connect(s, (sockaddr *) &peer, sizeof(peer))) {
        std::cerr << "connecting to " << server_addr << ":" << port << " failed" << std::endl;
        disconnect();
        return;
    }

    io = new SocketIO(s);
}

void FTPClientTCP::disconnect() {
    if (io) {
        io->sendRequest(Request::DISCONNECT);
        delete io;
        io = nullptr;
    }
    if (s > 0) {
        shutdown(s, SHUT_RDWR);
        close(s);
        s = 0;
    }
}

std::string FTPClientTCP::pwd() {
    validate();
    io->sendRequest(Request::PWD);
    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "pwd failed: " << response2string(response) << std::endl;
        return "";
    }
    return io->getString();
}

std::vector<FTPEntity> FTPClientTCP::ls() {
    validate();
    io->sendRequest(Request::LS);
    auto response = io->getResponse();
    std::vector<FTPEntity> entities;
    if (response != Response::OK) {
        std::cerr << "ls failed: " << response2string(response) << std::endl;
        return entities;
    }
    size_t size = io->getDataSize();
    for (auto i = 0; i < size; i++) {
        bool is_directory = io->getBool();
        std::string name = io->getString();
        entities.emplace_back(name, is_directory);
    }
    return entities;
}

void FTPClientTCP::cd(const std::string &path) {
    validate();
    io->sendRequest(Request::CD);
    io->sendString(path);

    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "cd failed: " << response2string(response) << std::endl;
        return;
    }
}

void FTPClientTCP::get(const std::string &file_name) {
    validate();
    io->sendRequest(Request::GET);
    io->sendString(file_name);

    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "get failed: " << response2string(response) << std::endl;
        return;
    }

    std::ofstream file(CLIENT_PATH + file_name);
    io->getFile(file);
}

void FTPClientTCP::put(const std::string &file_name) {
    validate();
    io->sendRequest(Request::PUT);
    io->sendString(file_name);

    std::ifstream file(CLIENT_PATH + file_name, std::ifstream::binary);
    io->sendFile(file);

    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "put failed: " << response2string(response) << std::endl;
        return;
    }
}

void FTPClientTCP::validate() {
    if (s <= 0 || !io) {
        throw std::runtime_error("socket is closed");
    }
}
