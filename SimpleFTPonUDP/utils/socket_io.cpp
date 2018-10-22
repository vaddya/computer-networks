#include "socket_io.h"

SocketIO::SocketIO(int socket) : s(socket) {
    in_buffer = new char[UDP_MAX_SIZE];
    out_buffer = new char[UDP_MAX_SIZE];
    file_buffer = new char[FILE_BUFFER_SIZE];
}

SocketIO::~SocketIO() {
    delete[] in_buffer;
    delete[] out_buffer;
    delete[] file_buffer;
}

ssize_t SocketIO::send(sockaddr_in peer, Package req) {
    size_t size = req.fillBuffer(out_buffer, UDP_MAX_SIZE);
    fd_set set;
    timeval timeout{};
    while (true) {
        std::cout << "Sending package " << req.toString() << std::endl;
        auto ns = sendto(this->s, out_buffer, size, 0, (sockaddr *) &peer, sizeof peer);
        if (ns < 0) {
            throw std::runtime_error("Cannot send in socket: " + std::to_string(s));
        }
        if (req.getType() == PackageType::ACK) {
            break;
        }
        FD_ZERO(&set);
        FD_SET(s, &set);
        timeout.tv_sec = 10;
        if (select(s + 1, &set, nullptr, nullptr, &timeout) == 0) {
            continue;
        }

        sockaddr_in from{};
        socklen_t from_size = sizeof from;
        Package resp = receive(reinterpret_cast<sockaddr *>(&from), &from_size);

//        if (from.sin_addr.s_addr != peer.sin_addr.s_addr || from.sin_port != peer.sin_port) {
//            std::cerr << "Package not for me, drop" << std::endl;
//            continue;
//        }

        if (resp.getCounter() < req.getCounter() + 1) {
            if (resp.getType() != PackageType::ACK) {
                std::cerr << "Got too old package, ack: " << resp.getCounter() << " instead of " << req.getCounter() + 1 << std::endl;
                send(peer, Package::ack(resp.getCounter() + 1, resp.getCounter()));
            }
        } else if (resp.getCounter() == req.getCounter() + 1) {
            if (resp.getType() == PackageType::ACK) {
                std::cout << "Got ack, drop" << std::endl;
                int ackFor;
                strncpy(reinterpret_cast<char *>(&ackFor), resp.getData(), sizeof ackFor);
                if (ackFor != req.getCounter()) {
                    throw std::runtime_error("Ack for: " + std::to_string(ackFor) + " instead of " + std::to_string(req.getCounter()));
                }
                break;
            } else {
                throw std::runtime_error("Wrong package type: " + type2string(resp.getType()) + " instead of ACK");
            }

        } else {
            std::cerr << "Got too new package, drop: " << resp.getCounter() << " instead of " << req.getCounter() + 1 << std::endl;
        }
    }
}

void SocketIO::sendFile(sockaddr_in peer, size_t counter, std::ifstream &file) {
    file.seekg(0, std::ifstream::end);
    auto size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ifstream::beg);

    size_t left = size;
    while (true) {
        if (left > FILE_BUFFER_SIZE) {
            file.read(file_buffer, FILE_BUFFER_SIZE);
            send(peer, Package::response(counter, Response::FILE_DATA, file_buffer, FILE_BUFFER_SIZE));
            left -= FILE_BUFFER_SIZE;
        } else {
            file.read(file_buffer, left);
            send(peer, Package::response(counter, Response::FILE_DATA_END, file_buffer, left));
            break;
        }
    }
}

Package SocketIO::receive(sockaddr *from, socklen_t *from_size) {
    ssize_t rc = recvfrom(s, in_buffer, UDP_MAX_SIZE, 0, from, from_size);
    if (rc < 0) {
        std::string err = strerror(errno);
        std::cout << err;
    }
    Package p(in_buffer, rc);
    std::cout << "Received package " << p.toString() << std::endl;
    return p;
}

Package SocketIO::receive() {
    return receive(nullptr, nullptr);
}

void SocketIO::receiveFile(std::ofstream &file) {
//    while (true) {
//        Package p = receive(nullptr, nullptr);
//        if (p.getType() == PackageType::FILE_DATA) {
//            file.write(p.getData(), p.getDataSize());
//        } else if (p.getType() == PackageType::FILE_DATA_END) {
//            file.write(p.getData(), p.getDataSize());
//            break;
//        }
//        throw std::runtime_error("Something went wrong");
//    }
}


