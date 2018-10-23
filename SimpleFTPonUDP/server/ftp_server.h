#ifndef SIMPLEFTPONUDP_SERVER_H
#define SIMPLEFTPONUDP_SERVER_H

#include <pthread.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include "response.h"
#include "socket_io.h"

namespace fs = std::experimental::filesystem;

class FTPServer {

public:
    FTPServer(SocketIO *io, sockaddr_in peer, socklen_t peer_size) : io(io), peer(peer), peer_size(peer_size) {
        path = std::string(SERVER_PATH);
    }

    ~FTPServer() {
        delete io;
    }

    void process_request(const Package &req) {
        if (req.getCounter() != counter) {
            std::cerr << "smth wrong with indexes: " << req.getCounter() << " instead of " << counter << std::endl;
        }
        counter += 1; // usr msg
        io->send(peer, Package::ack(counter, req.getCounter()));
        counter += 1; // our ack

        switch (req.getRequest()) {
            case Request::CONNECT:
                connect(req);
                break;
            case Request::PWD:
                pwd(req);
                break;
            case Request::LS:
                ls(req);
                break;
            case Request::CD:
                cd(req);
                break;
            case Request::GET:
                get(req);
                break;
            case Request::PUT:
                put(req);
                break;
            case Request::DISCONNECT:
                break;
            default:
                std::cerr << "Unknown request: " << req.getRequest() << std::endl;
                break;
        }
    }

private:

    void connect(const Package &req) {
        io->send(peer, Package::response(counter, Response::OK));
        counter += 2; // our msg + usr ack
    }

    void pwd(const Package &req) {
        io->send(peer, Package::response(counter, Response::OK, path.c_str(), path.string().size()));
        counter += 2; // our msg + usr ack
    }

    void ls(const Package &req) {
        size_t size = static_cast<size_t>(std::distance(fs::directory_iterator(path), fs::directory_iterator()));
        std::vector<std::string> list;
        for (auto &p : fs::directory_iterator(path)) {
            list.push_back(p.path().filename());
        }
        io->send(peer, Package::response(counter, Response::OK, list));
        counter += 2; // our msg + usr ack
    }

    void cd(const Package &req) {
        fs::path attempt = path / req.extractString();
        if (!fs::exists(attempt)) {
            io->send(peer, Package::response(counter, Response::NOT_EXISTS));
        } else if (!fs::is_directory(attempt)) {
            io->send(peer, Package::response(counter, Response::NOT_DIRECTORY));
        } else {
            io->send(peer, Package::response(counter, Response::OK));
            path = fs::canonical(attempt);
        }
        counter += 2; // our msg + usr ack
    }

    void get(const Package &req) {
        fs::path attempt = path / req.extractString();
        if (!fs::exists(attempt)) {
            io->send(peer, Package::response(counter, Response::NOT_EXISTS));
            counter += 2; // our msg + usr ack
        } else if (!fs::is_regular_file(attempt)) {
            io->send(peer, Package::response(counter, Response::NOT_REGULAR_FILE));
            counter += 2; // our msg + usr ack
        } else {
            io->send(peer, Package::response(counter, Response::OK));
            counter += 2; // our msg + usr ack
            attempt = fs::canonical(attempt);
            std::ifstream file(attempt);
            counter = io->sendFile(peer, counter, file);
        }
    }

    void put(const Package &req) {
        fs::path attempt = path / req.extractString();
        if (fs::exists(attempt)) {
            io->send(peer, Package::response(counter, Response::ALREADY_EXISTS));
            counter += 2; // our msg + usr ack
        } else {
            io->send(peer, Package::response(counter, Response::OK));
            counter += 2; // our msg + usr ack
            std::ofstream file(attempt);
            counter = io->receiveFile(peer, counter, file);
        }
    }

    const std::string SERVER_PATH = "/home/vaddya/FTP/Server/";

    size_t counter;

    sockaddr_in peer;

    socklen_t peer_size;

    SocketIO *io;

    fs::path path;
};


#endif //SIMPLEFTPONUDP_SERVER_H
