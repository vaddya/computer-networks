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

ssize_t SocketIO::sendTo(sockaddr_in peer, Package req) {
    size_t size = req.fillBuffer(out_buffer, UDP_MAX_SIZE);
    fd_set set;
    timeval timeout{};
    int retry = 5;
    while (retry > 0) {
        std::cout << "Sending package " << req.toString() << std::endl;
        auto ns = sendto(this->s, out_buffer, size, 0, (sockaddr *) &peer, sizeof peer);
//        if (req.getType() == PackageType::REQUEST && req.getRequest() == Request::PWD) { // oops: duplicate package
//            ns = sendto(this->s, out_buffer, size, 0, (sockaddr *) &peer, sizeof peer);
//        }
        retry--;
        if (ns < 0) {
            throw std::runtime_error("Cannot send in socket: " + std::to_string(s));
        }
        if (req.getType() == PackageType::ACK) {
            break;
        }
        FD_ZERO(&set);
        FD_SET(s, &set);
        timeout.tv_sec = 1;
        if (select(s + 1, &set, nullptr, nullptr, &timeout) == 0) {
            continue;
        }

        Package resp = receiveFrom(peer);
        if (resp.getType() == PackageType::RESPONSE && resp.getResponse() == Response::ERROR) {
            std::cerr << "Got error instead of ACK, something wrong!" << std::endl;
            sendTo(peer, Package::ack(resp.getCounter() + 1, resp.getCounter()));
            break;
        } else if (resp.getCounter() < req.getCounter() + 1) {
            if (resp.getType() != PackageType::ACK) {
                std::cerr << "Got too old package, ack: " << resp.getCounter() << " instead of "
                          << req.getCounter() + 1 << std::endl;
                sendTo(peer, Package::ack(resp.getCounter() + 1, resp.getCounter()));
            }
        } else if (resp.getCounter() == req.getCounter() + 1) {
            if (resp.getType() == PackageType::ACK) {
                size_t ackFor = resp.extractAck();
                if (ackFor == req.getCounter()) {
                    std::cout << "Got the right ack: " << std::to_string(ackFor) << std::endl;
                } else {
                    std::cerr << "Got the wrong ack: " << std::to_string(ackFor) << " instead of "
                              << std::to_string(req.getCounter()) << std::endl;
                }
                break;
            } else {
                throw std::runtime_error("Wrong package type: " + type2string(resp.getType()) + " instead of ACK");
            }
        } else {
            std::cerr << "Got too new package, drop: " << resp.getCounter() << " instead of "
                      << req.getCounter() + 1 << std::endl;
        }
    }
    if (retry == 0) {
        throw std::runtime_error("Cannot send request: " + req.toString());
    }
}

size_t SocketIO::sendFile(sockaddr_in peer, size_t counter, std::ifstream &file) {
    file.seekg(0, std::ifstream::end);
    auto size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ifstream::beg);

    size_t left = size;
    while (true) {
        if (left > FILE_BUFFER_SIZE) {
            file.read(file_buffer, FILE_BUFFER_SIZE);
            sendTo(peer, Package::response(counter, Response::FILE_DATA, file_buffer, FILE_BUFFER_SIZE));
            left -= FILE_BUFFER_SIZE;
        } else {
            file.read(file_buffer, left);
            sendTo(peer, Package::response(counter, Response::FILE_DATA_END, file_buffer, left));
            break;
        }
        counter += 2;
    }
    return counter;
}

Package SocketIO::receive(sockaddr *from, socklen_t *from_size) {
    ssize_t rc = recvfrom(s, in_buffer, UDP_MAX_SIZE, 0, from, from_size);
    if (rc <= 0) {
        std::cerr << "Cannot read from socket, errno: " << strerror(errno);
        return Package::empty();
    }
    return Package::parse(in_buffer, rc);
}

Package SocketIO::receiveFrom(sockaddr_in peer) {
    FTPPeer peerExpected(peer);
    sockaddr_in from{};
    socklen_t from_size = sizeof from;
    Package pack = receive(reinterpret_cast<sockaddr *>(&from), &from_size);
    FTPPeer peerFrom(from);
    if (peerExpected.equals(peerFrom)) {
        std::cout << "Received package " << pack.toString() << " from " << peerFrom.toString() << std::endl;
        return pack;
    } else {
        std::cerr << "Received package from stranger, drop: " << peerFrom.toString() << std::endl;
        return receiveFrom(peer);
    }
}

size_t SocketIO::receiveFile(sockaddr_in peer, size_t counter, std::ofstream &file) {
//    bool passed = false; // oops: drop package once
    while (true) {
        Package resp = receiveFrom(peer);
        counter += 1;
//        if (counter == 9 && !passed) {
//            passed = true;
//            counter -=1 ;
//            std::cout << "not sending ack" << std::endl;
//            continue;
//        }
        sendTo(peer, Package::ack(counter, resp.getCounter()));
        counter += 1;
        file.write(resp.getData(), resp.getDataSize());
        if (resp.getResponse() == Response::FILE_DATA) {
            continue;
        } else if (resp.getResponse() == Response::FILE_DATA_END) {
            break;
        }
        throw std::runtime_error("Wrong response: " + response2string(resp.getResponse()));
    }
    return counter;
}


