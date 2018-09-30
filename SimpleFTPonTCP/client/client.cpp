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
    *s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        throw std::runtime_error("socket call failed");
    }

    if (::connect(*s, (struct sockaddr *) &peer, sizeof(peer))) {
        throw std::runtime_error("Connecting failed");
    }

    io = new SocketIO(s);
}

void Client::disconnect() {
    if (s) {
        shutdown(*s, SHUT_RDWR);
        close(*s);
        delete io;
        io = nullptr;
        delete s;
        s = nullptr;
    }
}

std::vector<std::string> Client::ls() {
    io->sendRequest(Request::LS);
    auto response = io->getResponse();
    if (response != Response::OK) {
        throw std::runtime_error("LS failed");
    }
    size_t size = io->getDataSize();
    std::vector<std::string> names(size);
    for (auto i = 0; i < size; i++) {
        names[i] = io->getString();
    }
    return names;
}

void Client::cd(const std::string &path) {
    io->sendRequest(Request::CD);
    io->sendString(path);
}

void Client::get(const std::string &file_name) {
    io->sendRequest(Request::GET);
    io->sendString(file_name);

    auto response = io->getResponse();
    if (response != Response::OK) {
        throw std::runtime_error("Get failed: " + response);
    }

    std::ofstream file(file_name);
    io->getFile(file);
}

void Client::put(const std::string &file_name) {
    io->sendRequest(Request::PUT);
    io->sendString(file_name);

    std::ifstream file(file_name);
    io->sendFile(file);

    auto response = io->getResponse();
    if (response != Response::OK) {
        throw std::runtime_error("Put failed: " + response);
    }
}

int main(int argc, char **argv) {
    auto client = Client();

    std::string command;
    std::cin >> command;
    while (command != "exit") {
        if (command == "connect") {
            std::string host;
            std::cin >> host;
            int port;
            std::cin >> port;
            client.connect(host, port);
        } else if (command == "ls") {
            for (const auto &file : client.ls()) {
                std::cout << file << std::endl;
            }
        } else if (command == "cd") {
            std::string path;
            std::cin >> path;
            client.cd(path);
        } else if (command == "get") {
            std::string file_name;
            std::cin >> file_name;
            client.get(file_name);
        } else if (command == "put") {
            std::string file_name;
            std::cin >> file_name;
            client.put(file_name);
        } else if (command == "disconnect") {
            client.disconnect();
        } else {
            std::cout << "Supported commands: " << std::endl
                      << " - connect" << std::endl
                      << " - ls" << std::endl
                      << " - cd" << std::endl
                      << " - get" << std::endl
                      << " - put" << std::endl
                      << " - disconnect" << std::endl;
        }
        std::cin >> command;
    }

    return 0;
}
