#include "socket_io.h"

SocketIO::SocketIO(int *socket) {
    buffer = new char[BUFFER_SIZE];
    s = socket;
}

SocketIO::~SocketIO() {
    delete buffer;
}

ssize_t SocketIO::sendRequest(Request request) {
    return send(*this->s, &request, sizeof request, 0);
}

ssize_t SocketIO::sendResponse(Response response) {
    return send(*this->s, &response, sizeof response, 0);
}

ssize_t SocketIO::sendBool(bool b) {
    return send(*this->s, &b, sizeof b, 0);
}

ssize_t SocketIO::sendString(const std::string &string) {
    sendDataSize(string.size());
    return sendData(string.c_str(), string.size());
}

ssize_t SocketIO::sendDataSize(size_t size) {
    return send(*this->s, &size, sizeof size, 0);
}

ssize_t SocketIO::sendData(const char *buffer, size_t size) {
    return send(*this->s, buffer, size, 0);
}

Response SocketIO::getResponse() {
    Response response;
    auto rc = readn(*this->s, reinterpret_cast<char *>(&response), sizeof response);
    if (rc <= 0) {
        throw std::runtime_error("Cannot read response from socket: " + std::to_string(*this->s));
    }
    return response;
}

Request SocketIO::getRequest() {
    Request request;
    auto rc = readn(*this->s, reinterpret_cast<char *>(&request), sizeof request);
    if (rc <= 0) {
        throw std::runtime_error("Cannot read request from socket: " + std::to_string(*this->s));
    }
    return request;
}

bool SocketIO::getBool() {
    bool b;
    auto rc = readn(*this->s, reinterpret_cast<char *>(&b), sizeof b);
    if (rc <= 0) {
        throw std::runtime_error("Cannot read bool from socket: " + std::to_string(*this->s));
    }
    return b;
}

size_t SocketIO::getDataSize() {
    size_t size;
    readn(*this->s, reinterpret_cast<char *>(&size), sizeof size);
    return size;
}

ssize_t SocketIO::getData(char *buffer, size_t size) {
    return readn(*this->s, buffer, size);
}

std::string SocketIO::getString() {
    size_t size = getDataSize();
    getData(buffer, size);
    buffer[size] = '\0';
    return std::string(buffer);
}

void SocketIO::sendFile(std::ifstream &file) {
    file.seekg(0, std::ifstream::end);
    auto size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ifstream::beg);
    sendDataSize(size);

    size_t left = size;
    while (left > 0) {
        auto ns = left > BUFFER_SIZE ? BUFFER_SIZE : left;
        file.read(buffer, ns);
        sendData(buffer, ns);
        left -= ns;
    }
}

void SocketIO::getFile(std::ofstream &file) {
    auto size = getDataSize();
    size_t left = size;
    while (left > 0) {
        auto nr = left > BUFFER_SIZE ? BUFFER_SIZE : left;
        auto received = getData(buffer, nr);
        if (received <= 0) {
            throw std::runtime_error("Receive failed");
        }
        file.write(buffer, nr);
        left -= received;
    }
}
