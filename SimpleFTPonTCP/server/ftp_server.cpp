#include "ftp_server.h"

int main(int argc, char **argv) {
    struct sockaddr_in local{};
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
    int rc = bind(*ss, (struct sockaddr *) &local, sizeof(local));
    if (rc < 0) {
        std::cerr << "bind call failure" << std::endl;
        return 1;
    }
    rc = listen(*ss, 5);
    if (rc) {
        std::cerr << "listen call failed" << std::endl;
        return 1;
    }

    pthread_mutex_init(&mtx, nullptr);

    pthread_t accept_thread_id;
    pthread_create(&accept_thread_id, nullptr, &accept_thread, (void *) ss);

    std::string command;
    std::cin >> command;
    while (command != "exit") {
        if (command == "list") {
            pthread_mutex_lock(&mtx);
            std::cout << "Total " << servers.size() << " servers" << std::endl;
            for (int i = 0; i < servers.size(); i++) {
                std::cout << i << ": " << servers[i]->to_string() << std::endl;
            }
            pthread_mutex_unlock(&mtx);
        } else if (command == "kill") {
            int i;
            std::cin >> i;
            if (i >= 0 && i < servers.size()) {
                pthread_mutex_lock(&mtx);
                servers[i]->kill_and_join();
                servers.erase(servers.begin() + i);
                pthread_mutex_unlock(&mtx);
            } else {
                std::cout << "Wrong client index" << std::endl;
            }
        } else {
            std::cout << "Supported commands: " << std::endl
                      << " - list" << std::endl
                      << " - kill x" << std::endl
                      << " - exit" << std::endl;
        }
        std::cin >> command;
    }
    shutdown(*ss, SHUT_RDWR);
    close(*ss);
    pthread_join(accept_thread_id, nullptr);
    delete ss;
    return 0;
}

void *client_thread(void *data) {
    auto server = reinterpret_cast<Server *>(data);
    try {
        server->processRequests();
    } catch (std::exception &e) {
        std::cout << "Got exception " << e.what() << std::endl;
    }
}

void *accept_thread(void *data) {
    auto ss = reinterpret_cast<int *>(data);
    while (true) {
        auto cs = new int;
        *cs = accept(*ss, nullptr, nullptr);
        if (*cs < 0) {
            std::cout << "Cannot accept connection" << std::endl;
            break;
        }
        auto thread_id = new pthread_t;
        auto server = new Server(cs, thread_id);
        pthread_create(thread_id, nullptr, &client_thread, (void *) server);
        pthread_mutex_lock(&mtx);
        servers.push_back(server);
        pthread_mutex_unlock(&mtx);
        std::cout << "Joined client with socket " << *cs << std::endl;
    }
    std::cout << "Stop accepting connections" << std::endl;
    pthread_mutex_lock(&mtx);
    for (auto s : servers) {
        s->kill_and_join();
    }
    servers.clear();
    pthread_mutex_unlock(&mtx);
    pthread_mutex_destroy(&mtx);
}