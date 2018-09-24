#include "server.h"

int main(int argc, char **argv) {
    struct sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    int ss = socket(AF_INET, SOCK_STREAM, 0);
    if (ss < 0) {
        perror("socket call failed");
        return 1;
    }
    int enable = 1;
    setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    int rc = bind(ss, (struct sockaddr *) &local, sizeof(local));
    if (rc < 0) {
        perror("bind call failure");
        return 1;
    }
    rc = listen(ss, 5);
    if (rc) {
        perror("listen call failed");
        return 1;
    }

    auto ssp = new int;
    *ssp = ss;
    pthread_t accept_thread_id;
    pthread_create(&accept_thread_id, nullptr, &accept_thread, (void *) ssp);

    std::string command;
    while (true) {
        std::cin >> command;
        if (command == "list") { // список подключенных клиентов
            pthread_mutex_lock(&clients_mutex);
            std::cout << "Total " << clients.size() << " clients" << std::endl;
            for (int i = 0; i < clients.size(); i++) {
                std::cout << i << ": socket id=" << clients[i].socket << std::endl;
            }
            pthread_mutex_unlock(&clients_mutex);
        } else if (command == "kill") { // отключить i-ого клииента
            int i;
            std::cin >> i;
            if (i >= 0 && i < clients.size()) {
                pthread_mutex_lock(&clients_mutex);
                kill_client(clients[i]);
                clients.erase(clients.begin() + i);
                pthread_mutex_unlock(&clients_mutex);
            } else {
                std::cout << "Wrong client index" << std::endl;
            }
        } else if (command == "exit") { // выход
            shutdown(ss, SHUT_RDWR);
            close(ss);
            pthread_join(accept_thread_id, nullptr);
            break;
        } else {
            std::cout << "Supported commands: " << std::endl
                      << " - list" << std::endl
                      << " - kill x" << std::endl
                      << " - exit" << std::endl;
        }
    }
    return 0;
}

void kill_client(Client &c) {
    std::cout << "Killing client with socket: " << c.socket << std::endl;
    shutdown(c.socket, SHUT_RDWR);
    close(c.socket);
    pthread_join(*c.thread_id, nullptr);
    delete c.thread_id;
    c.thread_id = nullptr;
}

void *client_thread(void *data) {
    auto csp = reinterpret_cast<int *>(data);
    int cs = *csp;
    char buf[BUFFER_SIZE + 1];
    buf[BUFFER_SIZE] = '\0';
    while (true) {
        int read = readn(cs, buf, BUFFER_SIZE);
        if (read < 0) {
            std::cout << "Cannot read from socket: " << cs << std::endl;
            break;
        }
        std::cout << "Receive from socket " << cs << ": " << buf << std::endl;
        ssize_t rc = send(cs, buf, BUFFER_SIZE, MSG_NOSIGNAL);
        if (rc < 0) {
            std::cout << "Sending error to socket: " << cs << std::endl;
        }
    }
    delete csp;
}

void *accept_thread(void *data) {
    auto ssp = reinterpret_cast<int *>(data);
    int ss = *ssp;
    int cs;
    pthread_mutex_init(&clients_mutex, nullptr);
    while (true) {
        cs = accept(ss, nullptr, nullptr);
        auto csp = new int;
        *csp = cs;
        if (cs < 0) {
            std::cout << "Cannot accept connection" << std::endl;
            break;
        }
        auto thread_id = new pthread_t;
        pthread_create(thread_id, nullptr, &client_thread, (void *) csp);
        pthread_mutex_lock(&clients_mutex);
        clients.push_back({cs, thread_id});
        pthread_mutex_unlock(&clients_mutex);
        std::cout << "Joined client with socket " << cs << std::endl;
    }
    std::cout << "Stop accepting connections" << std::endl;
    pthread_mutex_lock(&clients_mutex);
    for (Client c : clients) {
        kill_client(c);
    }
    clients.clear();
    pthread_mutex_unlock(&clients_mutex);
    pthread_mutex_destroy(&clients_mutex);
    delete ssp;
}
