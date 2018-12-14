#include "ftp_client_udp.h"

FTPClientUDP::FTPClientUDP() {
    s = 0;
    io = nullptr;
}

FTPClientUDP::~FTPClientUDP() {
    disconnect();
}

void FTPClientUDP::connect(const std::string &server_addr, int port) {
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
    io->sendTo(peer, Package::request(counter, Request::CONNECT));
    counter += 1;
    Package resp = io->receiveFrom(peer);
    if (resp.getCounter() != counter + 1) {
        std::cerr << "wrong resp counter" << std::endl;
        return;
    }
    counter += 2;
    io->sendTo(peer, Package::ack(counter, resp.getCounter()));
    counter += 1;
    if (resp.getResponse() != Response::OK) {
        std::cerr << "connect failed: " << response2string(resp.getResponse());
    }
    std::cout << "connection done" << std::endl;
}

void FTPClientUDP::disconnect() {
    if (io) {
        io->sendTo(peer, Package::request(counter, Request::DISCONNECT));
        delete io;
        io = nullptr;
    }
    if (s > 0) {
        shutdown(s, SHUT_RDWR);
        close(s);
        s = 0;
    }
}

std::string FTPClientUDP::pwd() {
    validate();
//    counter += 100; // oops: wrong package counter
    io->sendTo(peer, Package::request(counter, Request::PWD));
    Package resp = io->receiveFrom(peer);
    counter += 3; // our msg + srv ack + srv resp
    io->sendTo(peer, Package::ack(counter, resp.getCounter()));
    counter += 1; // our ack
    if (resp.getResponse() != Response::OK) {
        std::cerr << "pwd failed: " << response2string(resp.getResponse()) << std::endl;
        return "";
    }
    return resp.extractString();
}

std::vector<FTPEntity> FTPClientUDP::ls() {
    validate();
    io->sendTo(peer, Package::request(counter, Request::LS));
    Package resp = io->receiveFrom(peer);
    counter += 3; // our msg + srv ack + srv resp
    io->sendTo(peer, Package::ack(counter, resp.getCounter()));
    counter += 1; // our ack
    std::vector<FTPEntity> entities;
    if (resp.getResponse() != Response::OK) {
        std::cerr << "ls failed: " << response2string(resp.getResponse()) << std::endl;
        return entities;
    }
    std::vector<std::string> list = resp.extractList();
    for (auto &s : list) {
        entities.emplace_back(s, false);
    }
    return entities;
}

void FTPClientUDP::cd(const std::string &path) {
    validate();
    io->sendTo(peer, Package::request(counter, Request::CD, path.c_str(), path.size() + 1));
    Package resp = io->receiveFrom(peer);
    counter += 3; // our msg + srv ack + srv resp
    io->sendTo(peer, Package::ack(counter, resp.getCounter()));
    counter += 1; // our ack

    if (resp.getResponse() != Response::OK) {
        std::cerr << "cd failed: " << response2string(resp.getResponse()) << std::endl;
        return;
    } else {
        std::cout << "done" << std::endl;
    }
}

void FTPClientUDP::get(const std::string &file_name) {
    validate();
    io->sendTo(peer, Package::request(counter, Request::GET, file_name.c_str(), file_name.size()));
    Package resp = io->receiveFrom(peer);
    counter += 3; // our msg + srv ack + srv resp
    io->sendTo(peer, Package::ack(counter, resp.getCounter()));
    counter += 1; // our ack
    if (resp.getResponse() != Response::OK) {
        std::cerr << "get failed: " << response2string(resp.getResponse()) << std::endl;
        return;
    }
    std::ofstream file(CLIENT_PATH + file_name, std::ifstream::binary);
    counter = io->receiveFile(peer, counter, file);
    std::cout << "done" << std::endl;
}

void FTPClientUDP::put(const std::string &file_name) {
    validate();
    io->sendTo(peer, Package::request(counter, Request::PUT, file_name.c_str(), file_name.size()));
    Package resp = io->receiveFrom(peer);
    counter += 3; // our msg + srv ack + srv resp
    io->sendTo(peer, Package::ack(counter, resp.getCounter()));
    counter += 1; // our ack
    if (resp.getResponse() != Response::OK) {
        std::cerr << "get failed: " << response2string(resp.getResponse()) << std::endl;
        return;
    }
    std::ifstream file(CLIENT_PATH + file_name, std::ifstream::binary);
    counter = io->sendFile(peer, counter, file);
    std::cout << "done" << std::endl;
}

void FTPClientUDP::validate() {
    if (s <= 0 || !io) {
        throw std::runtime_error("socket is closed");
    }
}
