#include "server.h"

int main(int argc, char **argv) {
    sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    auto ss = new int;
    *ss = socket(AF_INET, SOCK_STREAM, 0);
    if (*ss < 0) {
        std::cerr << "socket call failed" << std::endl;
        return 1;
    }
    int enable = 1;
    setsockopt(*ss, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    int rc = bind(*ss, (sockaddr *) &local, sizeof(local));
    if (rc < 0) {
        std::cerr << "Bind call failure" << std::endl;
        return 1;
    }
    rc = listen(*ss, 5);
    if (rc) {
        std::cerr << "Listen call failed" << std::endl;
        return 1;
    }

    pthread_rwlock_init(&lock, nullptr);

    pthread_t accept_thread_id;
    pthread_create(&accept_thread_id, nullptr, &acceptThread, (void *) ss);

    std::string command;
    std::cin >> command;
    while (command != "exit") {
        if (command == "list") {
            pthread_rwlock_rdlock(&lock);
            std::cout << "Total " << servers.size() << " clients" << std::endl;
            for (int i = 0; i < servers.size(); i++) {
                std::cout << i << ": " << servers[i]->toString() << std::endl;
            }
            pthread_rwlock_unlock(&lock);
        } else if (command == "kill") {
            int i;
            std::cin >> i;
            pthread_rwlock_wrlock(&lock);
            if (i >= 0 && i < servers.size()) {
                servers[i]->disconnect();
            } else {
                std::cerr << "Wrong client index" << std::endl;
            }
            pthread_rwlock_unlock(&lock);
        } else {
            std::cout << SERVER_HELP << std::endl;
        }
        std::cin >> command;
    }
    pthread_rwlock_wrlock(&lock);
    for (auto s : servers) {
        s->disconnect();
    }
    pthread_rwlock_unlock(&lock);
    shutdown(*ss, SHUT_RDWR);
    close(*ss);
    pthread_join(accept_thread_id, nullptr);
    pthread_rwlock_destroy(&lock);
    delete ss;
    return 0;
}

void *acceptThread(void *data) {
    auto ss = reinterpret_cast<int *>(data);
    while (true) {
        auto cs = accept(*ss, nullptr, nullptr);
        if (cs < 0) {
            std::cerr << "Cannot accept connection" << std::endl;
            break;
        }
        auto thread_id = new pthread_t;
        auto server = new FTPServerTCP(cs, thread_id);
        pthread_create(thread_id, nullptr, &clientThread, (void *) server);
        pthread_rwlock_wrlock(&lock);
        servers.push_back(server);
        pthread_rwlock_unlock(&lock);
        std::cout << "Joined client with socket " << cs << std::endl;
    }
    std::cout << "Stopped accepting connections" << std::endl;
}

void *clientThread(void *data) {
    auto server = reinterpret_cast<FTPServerTCP *>(data);
    try {
        server->processRequests();
    } catch (std::exception &e) {
        std::cerr << "Got exception " << e.what() << std::endl;
    }
    pthread_rwlock_wrlock(&lock);
    for (auto it = servers.begin(); it != servers.end(); it++) {
        if (*it == server) {
            delete *it;
            servers.erase(it);
            break;
        }
    }
    pthread_rwlock_unlock(&lock);
}
