#include "client.h"

Client::Client() {
    s = nullptr;
    buffer = new char[BUFFER_SIZE];
}

Client::~Client() {
    disconn();
    delete(buffer);
}

void Client::conn(std::string server_addr, int port) {
    sockaddr_in peer{};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(port);
    if (server_addr != nullptr) {
        peer.sin_addr.s_addr = inet_addr(server_addr.c_str());
    } else {
        peer.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    }
    *s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        throw std::runtime_error("socket call failed");
    }

    if (connect(*s, (struct sockaddr *) &peer, sizeof(peer))) {
        throw std::runtime_error("connect call failed");
    }
}

void Client::disconn() {
    if (s) {
        shutdown(*s, SHUT_RDWR);
        close(*s);
        delete s;
        s = nullptr;
    }
}

std::vector<std::string> Client::ls() {
    return std::vector<std::string>();
}

void Client::cd(std::string path) {

}

void Client::get(std::string file_name, std::ofstream dst) {

}

void Client::put(std::string file_name, std::ifstream src) {
    int rc = send(s, sendBuf, strlen(sendBuf), 0);
    if (rc <= 0) {
        perror("send call failed");
    }

    rc = recv(s, recvBuf, BUFFER_SIZE, 0);
    if (rc <= 0)
        perror("recv call failed");
    else
        printf("client recv: %s\n", recvBuf);
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
            client.conn(host, port);
        } else if (command == "ls") {
            for (const auto &file : client.ls()) {
                std::cout << file << std::endl;
            }
        } else if (command == "cd") {
            std::string path;
            std::cin >> path;
            client.cd(path);
        } else if (command == "get") {

        } else if (command == "put") {

        } else if (command == "disconnect") {

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
