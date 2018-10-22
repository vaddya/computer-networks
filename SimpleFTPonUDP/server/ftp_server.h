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
//        io->sendResponse(Response::OK);
//        size_t size = static_cast<size_t>(std::distance(
//                fs::directory_iterator(path), fs::directory_iterator())
//        );
//        io->sendDataSize(size);
//        for (auto &p : fs::directory_iterator(path)) {
//            io->sendBool(fs::is_directory(p.path()));
//            std::string str = p.path().filename();
//            io->sendString(str);
//        }
    }

    void cd(const Package &req) {
//        fs::path attempt = path / io->getString();
//        if (!fs::exists(attempt)) {
//            io->sendResponse(Response::NOT_EXISTS);
//        } else if (!fs::is_directory(attempt)) {
//            io->sendResponse(Response::NOT_DIRECTORY);
//        } else {
//            io->sendResponse(Response::OK);
//            path = fs::canonical(attempt);
//        }
    }

    void get(const Package &req) {
//        fs::path attempt = path / io->getString();
//        if (!fs::exists(attempt)) {
//            io->send(Package::response(Response::NOT_EXISTS));
//        } else if (!fs::is_regular_file(attempt)) {
//            io->send(Package::response(Response::NOT_REGULAR_FILE));
//        } else {
//            io->send(Package::response(Response::OK, ))
//            attempt = fs::canonical(attempt);
//            std::ifstream file(attempt);
//            io->send(file);
//        }
    }

    void put(const Package &req) {
//        fs::path attempt = path / io->getString();
//        if (fs::exists(attempt)) {
//            io->sendResponse(Response::ALREADY_EXISTS);
//        } else {
//            io->sendResponse(Response::OK);
//            std::ofstream file(attempt);
//            io->receive(file);
//        }
    }

    const std::string SERVER_PATH = "/home/vaddya/FTP/Server/";

    size_t counter;

    sockaddr_in peer;

    socklen_t peer_size;

    SocketIO *io;

    fs::path path;
};


#endif //SIMPLEFTPONUDP_SERVER_H
