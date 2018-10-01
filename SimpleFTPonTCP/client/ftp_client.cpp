#include "ftp_client.h"

int main(int argc, char **argv) {
    auto client = Client();

    client.connect("localhost", 7000);

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
            std::cout << client.pwd() << std::endl;
        } else if (command == "ls") {
            for (const auto &file : client.ls()) {
                std::cout << (file.first ? "[d] " : "[-] ") << file.second << std::endl;
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
                      << " - pwd" << std::endl
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