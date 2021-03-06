#include "client.h"

int main(int argc, char **argv) {
    auto client = FTPClientUDP();
    client.connect("localhost", PORT);

    std::string command;
    std::cin >> command;
    while (command != "exit") {
        if (command == "connect") {
            std::string host;
            std::cin >> host;
            int port;
            std::cin >> port;
            client.connect(host, port);
        } else if (command == "pwd") {
            std::string pwd = client.pwd();
            std::cout << "> " << pwd << std::endl;
        } else if (command == "ls") {
            for (const auto &entity : client.ls()) {
                std::cout << "> " << entity.name() << std::endl;
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
            std::cout << CLIENT_HELP << std::endl;
        }
        std::cin >> command;
    }
    return 0;
}