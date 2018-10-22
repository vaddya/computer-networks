#include "ftp_client.h"

FTPClient::FTPClient() {
    s = 0;
    io = nullptr;
}

FTPClient::~FTPClient() {
    disconnect();
}

void FTPClient::connect(const std::string &server_addr, int port) {
    disconnect();
    counter = 0;
    peer.sin_family = AF_INET;
    peer.sin_port = htons(static_cast<uint16_t>(port));
    if (server_addr != "localhost") {
        peer.sin_addr.s_addr = inet_addr(server_addr.c_str());
    } else {
        peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        std::cerr << "socket call failed" << std::endl;
        return;
    }

    io = new SocketIO(s);
    io->send(peer, Package::request(counter, Request::CONNECT));
    counter += 1;
    Package resp = io->receive();
    if (resp.getCounter() != counter + 1) {
        std::cerr << "wrong resp counter" << std::endl;
        return;
    }
    counter += 2;
    io->send(peer, Package::ack(counter, resp.getCounter()));
    counter += 1;
    if (resp.getResponse() != Response::OK) {
        std::cerr << "connect failed: " << response2string(resp.getResponse());
    }
}

void FTPClient::disconnect() {
    if (io) {
        io->send(peer, Package::request(counter, Request::DISCONNECT));
        delete io;
        io = nullptr;
    }
    if (s > 0) {
        shutdown(s, SHUT_RDWR);
        close(s);
        s = 0;
    }
}

std::string FTPClient::pwd() {
    validate();
    io->send(peer, Package::request(counter, Request::PWD));
    Package resp = io->receive(nullptr, nullptr);
    counter = resp.getCounter() + 1;
    io->send(peer, Package::ack(counter, resp.getCounter()));
    counter += 1;
    if (resp.getResponse() != Response::OK) {
        std::cerr << "pwd failed: " << response2string(resp.getResponse()) << std::endl;
        return "";
    }
    return std::string(resp.getData(), resp.getDataSize());
}

std::vector<FTPEntity> FTPClient::ls() {
    validate();
    io->send(peer, Package::request(counter++, Request::LS));
    Package p = io->receive(nullptr, nullptr);
    std::vector<FTPEntity> entities;
    if (p.getResponse() != Response::OK) {
        std::cerr << "ls failed: " << response2string(p.getResponse()) << std::endl;
        return entities;
    }
//    size_t size = io->getDataSize();
//    for (auto i = 0; i < size; i++) {
//        bool is_directory = io->getBool();
//        std::string name = io->getString();
//        entities.emplace_back(name, is_directory);
//    }
    return entities;
}

void FTPClient::cd(const std::string &path) {
    validate();
    io->send(peer, Package::request(counter++, Request::CD, path.c_str(), path.size()));

    Package p = io->receive();

    if (p.getResponse() != Response::OK) {
        std::cerr << "cd failed: " << response2string(p.getResponse()) << std::endl;
        return;
    }
}

void FTPClient::get(const std::string &file_name) {
    validate();
//    io->sendRequest(Request::GET);
//    io->sendString(file_name);
//
//    auto response = io->getResponse();
//    if (response != Response::OK) {
//        std::cerr << "get failed: " << response2string(response) << std::endl;
//        return;
//    }
//
//    std::ofstream file(CLIENT_PATH + file_name);
//    io->receive(file);
}

void FTPClient::put(const std::string &file_name) {
    validate();
//    io->sendRequest(Request::PUT);
//    io->sendString(file_name);
//
//    std::ifstream file(CLIENT_PATH + file_name, std::ifstream::binary);
//    io->send(file);
//
//    auto response = io->getResponse();
//    if (response != Response::OK) {
//        std::cerr << "put failed: " << response2string(response) << std::endl;
//        return;
//    }
}

void FTPClient::validate() {
    if (s <= 0 || !io) {
        throw std::runtime_error("socket is closed");
    }
}
