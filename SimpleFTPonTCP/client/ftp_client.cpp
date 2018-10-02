#include "ftp_client.h"

FTPClient::FTPClient() {
    s = 0;
}

FTPClient::~FTPClient() {
    disconnect();
}

void FTPClient::connect(const std::string &server_addr, int port) {
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

    if (::connect(s, (struct sockaddr *) &peer, sizeof(peer))) {
        std::cerr << "connecting to " << server_addr << ":" << port << " failed" << std::endl;
        return;
    }

    io = new SocketIO(s);
}

void FTPClient::disconnect() {
    if (s) {
        io->sendRequest(Request::DISCONNECT);
        shutdown(s, SHUT_RDWR);
        close(s);
    }
    delete io;
}

std::string FTPClient::pwd() {
    validate();
    io->sendRequest(Request::PWD);
    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "pwd failed: " << response2string(response) << std::endl;
        return "";
    }
    return io->getString();
}

std::vector<FTPEntity> FTPClient::ls() {
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

void FTPClient::cd(const std::string &path) {
    validate();
    io->sendRequest(Request::CD);
    io->sendString(path);

    auto response = io->getResponse();
    if (response != Response::OK) {
        std::cerr << "cd failed: " << response2string(response) << std::endl;
        return;
    }
}

void FTPClient::get(const std::string &file_name) {
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

void FTPClient::put(const std::string &file_name) {
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

void FTPClient::validate() {
    if (s <= 0 || !io) {
        throw std::runtime_error("socket is closed");
    }
}
