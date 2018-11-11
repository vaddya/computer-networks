#include "server.h"

const int PORT = 7000;

int main(int argc, char **argv) {
    sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        std::cerr << "socket call failed" << std::endl;
        return 1;
    }
    auto enable = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    auto rc = bind(s, (sockaddr *) &local, sizeof(local));
    if (rc < 0) {
        std::cerr << "Bind call failure" << std::endl;
        return 1;
    }

    pthread_rwlock_init(&lock, nullptr);

    pthread_t proc_thread_id;
    pthread_create(&proc_thread_id, nullptr, &proc_thread, (void *) &s);

    std::string command;
    std::cin >> command;
    while (command != "exit") {
        if (command == "list") {
            std::cout << "Total " << servers.size() << " clients" << std::endl;
            pthread_rwlock_wrlock(&lock);
            for (auto kv : servers) {
                std::cout << kv.first.toString() << std::endl;
            }
            pthread_rwlock_unlock(&lock);
        } else if (command == "kill") {
            std::string str;
            std::cin >> str;
            pthread_rwlock_wrlock(&lock);
            auto it = servers.begin();
            while (it != servers.end()) {
                if (it->first.toString() == str) {
                    delete *it;
                    servers.erase(it);
                    break;
                }
                ++it;
            }
            pthread_rwlock_unlock(&lock);
        } else {
            std::cout << SERVER_HELP << std::endl;
        }
        std::cin >> command;
    }
    shutdown(s, SHUT_RDWR);
    close(s);
    pthread_join(proc_thread_id, nullptr);
    pthread_rwlock_destroy(&lock);
    return 0;
}

void *proc_thread(void *data) {
    auto s = reinterpret_cast<int *>(data);
    auto io = new SocketIO(*s);
    sockaddr_in from{};
    socklen_t from_size = sizeof from;
    Package req = io->receive(reinterpret_cast<sockaddr *>(&from), &from_size);
    while (req.getType() != PackageType::EMPTY) {
        Peer peer(from);
        if (servers.find(peer) == servers.end()) {
            pthread_rwlock_rdlock(&lock);
            servers[peer] = new FTPServer(io, from, from_size);
            pthread_rwlock_unlock(&lock);
            std::cout << "Client joined: " << peer.toString() << std::endl;
        }
        std::cout << "Received package " << req.toString() << " from " << peer.toString() << std::endl;
        pthread_rwlock_rdlock(&lock);
        servers[peer]->process_request(req);
        pthread_rwlock_unlock(&lock);
        if (req.getType() == PackageType::REQUEST && req.getRequest() == Request::DISCONNECT &&
            servers.find(peer) != servers.end()) {
            pthread_rwlock_wrlock(&lock);
            delete servers[peer];
            servers.erase(peer);
            pthread_rwlock_unlock(&lock);
            std::cout << "Client left: " << peer.toString() << std::endl;
        }
        req = io->receive(reinterpret_cast<sockaddr *>(&from), &from_size);
    }
    std::cout << "Stopped processing" << std::endl;
    delete io;
    return nullptr;
}