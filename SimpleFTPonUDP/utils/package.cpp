#include "package.h"

Package::Package(size_t counter, PackageType type, unsigned char reqresp, const char *data, size_t date_size) :
        counter(counter),
        type(type),
        reqresp(reqresp),
        data_size(date_size) {
    this->data = std::shared_ptr<char[]>(new char[data_size]);
    memcpy(this->data.get(), data, data_size);
}

Package::Package(char *buffer, ssize_t size) {
    assert(size >= counter_size + type_size + reqresp_size);
    memcpy(&counter, buffer, counter_size);
    memcpy(&type, buffer + counter_size, type_size);
    memcpy(&reqresp, buffer + counter_size + type_size, reqresp_size);
    data_size = size - counter_size - type_size - reqresp_size;
    data = std::shared_ptr<char[]>(new char[data_size]);
    memcpy(data.get(), buffer + counter_size + type_size + reqresp_size, data_size);
}

Package Package::parse(char *buffer, ssize_t size) {
    return Package(buffer, size);
}

Package Package::request(size_t counter, Request request, const char *data, size_t data_size) {
    return Package(counter, PackageType::REQUEST, request, data, data_size);
}

Package Package::request(size_t counter, Request request) {
    return Package(counter, PackageType::REQUEST, request, nullptr, 0);
}

Package Package::response(size_t counter, Response response, const char *data, size_t data_size) {
    return Package(counter, PackageType::RESPONSE, response, data, data_size);
}

Package Package::response(size_t counter, Response response, std::vector<std::string> list) {
    size_t count = list.size();
    size_t buffer_size = sizeof counter;
    std::for_each(list.begin(), list.end(), [&buffer_size](std::string str) {
        buffer_size += str.size() + 1;
    });
    char buffer[buffer_size];
    memcpy(buffer, &count, sizeof counter);
    char *it = buffer + sizeof counter;
    std::for_each(list.begin(), list.end(), [&it](std::string str) {
        memcpy(it, str.c_str(), str.size() + 1);
        it += str.size() + 1;
    });
    return Package(counter, PackageType::RESPONSE, response, buffer, buffer_size);
}

Package Package::response(size_t counter, Response response) {
    return Package(counter, PackageType::RESPONSE, response, nullptr, 0);
}

Package Package::ack(size_t counter, size_t n) {
    return Package(counter, PackageType::ACK, 0, reinterpret_cast<char *>(&n), sizeof n);
}

Package Package::empty() {
    return Package(0, PackageType::EMPTY, 0, nullptr, 0);
}

size_t Package::fillBuffer(char *buffer, size_t buffer_size) const {
    assert(buffer_size >= counter_size + type_size + reqresp_size + data_size);
    memcpy(buffer, &counter, counter_size);
    memcpy(buffer + counter_size, &type, type_size);
    memcpy(buffer + counter_size + type_size, &reqresp, reqresp_size);
    memcpy(buffer + counter_size + type_size + reqresp_size, data.get(), data_size);
    return counter_size + type_size + reqresp_size + data_size;
}

size_t Package::getCounter() const {
    return counter;
}

PackageType Package::getType() const {
    return type;
}

Request Package::getRequest() const {
    assert(type == PackageType::REQUEST);
    return static_cast<Request>(reqresp);
}

Response Package::getResponse() const {
    assert(type == PackageType::RESPONSE);
    return static_cast<Response>(reqresp);
}

char *Package::getData() const {
    return data.get();
}

size_t Package::getDataSize() const {
    return data_size;
}

size_t Package::extractAck() const {
    assert(type == PackageType::ACK);
    size_t ack;
    assert(data_size >= sizeof ack);
    memcpy(&ack, data.get(), sizeof ack);
    return ack;
}

std::vector<std::string> Package::extractList() const {
    std::vector<std::string> list;
    size_t size;
    memcpy(&size, data.get(), sizeof size);
    char *it = data.get() + sizeof size;
    for (int i = 0; i < size; i++) {
        size_t len = strlen(it);
        list.emplace_back(it, len);
        it += len + 1;
    }
    return list;
}

std::string Package::extractString() const {
    return std::string(data.get(), data_size);
}

std::string Package::toString() const {
    switch (type) {
        case PackageType::ACK:
            return "[#" + std::to_string(counter) + ", " + type2string(type) + " for " +
                   std::to_string(extractAck()) + ", " + std::to_string(data_size) + " bytes]";
        case PackageType::REQUEST:
            return "[#" + std::to_string(counter) + ", " + type2string(type) + ", " +
                   request2string(static_cast<Request>(reqresp)) + ", " + std::to_string(data_size) + " bytes]";
        case PackageType::RESPONSE:
            return "[#" + std::to_string(counter) + ", " + type2string(type) + ", " +
                   response2string(static_cast<Response >(reqresp)) + ", " + std::to_string(data_size) + " bytes]";
        case PackageType ::EMPTY:
            return "[EMPTY]";
    }
}
