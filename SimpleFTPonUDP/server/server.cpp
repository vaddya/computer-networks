#include "server.h"

int main(int argc, char **argv) {
    sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    auto ss = new int;
    *ss = socket(AF_INET, SOCK_DGRAM, 0);
    if (*ss < 0) {
        std::cerr << "socket call failed" << std::endl;
        return 1;
    }
    int enable = 1;
    setsockopt(*ss, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    auto rc = bind(*ss, (sockaddr *) &local, sizeof(local));
    if (rc < 0) {
        std::cerr << "Bind call failure" << std::endl;
        return 1;
    }

    pthread_rwlock_init(&lock, nullptr);

    pthread_t proc_thread_id;
    pthread_create(&proc_thread_id, nullptr, &proc_thread, (void *) ss);

    std::string command;
    std::cin >> command;
    while (command != "exit") {
        if (command == "list") {
            std::cout << "Total " << servers.size() << " clients" << std::endl;
            for (auto kv : servers) {
                std::cout << kv.first.toString() << std::endl;
            }
        } else if (command == "kill") {
            std::string str;
            std::cin >> str;
            for (auto kv : servers) {
                if (kv.first.toString() == str) {
                    // TODO: erase
                }
            }
        } else {
            std::cout << SERVER_HELP << std::endl;
        }
        std::cin >> command;
    }
    end = true;
    pthread_join(proc_thread_id, nullptr);
    pthread_rwlock_destroy(&lock);
    shutdown(*ss, SHUT_RDWR);
    close(*ss);
    return 0;
}

void *proc_thread(void *data) {
    auto s = reinterpret_cast<int *>(data);
    sockaddr_in from{};
    socklen_t from_size = sizeof from;
    auto io = new SocketIO(*s);
    while (!end) {
        Package req = io->receive(reinterpret_cast<sockaddr *>(&from), &from_size);
        Peer p(from);
        if (servers.find(p) == servers.end()) {
            pthread_rwlock_wrlock(&lock);
            servers[p] = new FTPServer(io, from, from_size);
            pthread_rwlock_unlock(&lock);
            std::cout << "Joined client: " << p.toString() << std::endl;
        }
        servers[p]->process_request(req);
    }
    delete io;
    std::cout << "Stopped processing" << std::endl;
}